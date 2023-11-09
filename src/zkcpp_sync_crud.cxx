
#include <pthread.h>
#include <algorithm>
#include <assert.h>
#include "zkcpp.h"
#include "zkcpp_conf.h"
#include "zkcpp_macro.h"
#include "zkcpp_func.h"
// 初始化zookeeper客户端
ZookeeperClient::ZookeeperClient(int timout, std::string host)
{
    CConfig *config_instance = CConfig::GetInstance();
    if(host == "")
    {
        config_instance->Load("../zkcpp.conf");
        const char *ip = config_instance->GetString("IP");
        const char *port = config_instance->GetString("PORT");
        host = std::string(ip) + ":" + std::string(port);
        if(host != "")
            zkcpp_log_error_core(ZKCPP_LOG_INFO, 0, "success get host");
    }
    zkcpp_log_init();
    if(timout == 0)
    {
        timout = config_instance->GetIntDefault("TIMEOUT", 3000);
        zkcpp_log_error_core(ZKCPP_LOG_INFO, 0, "success get timeout-config %d", timout);
    }
    try_times = config_instance->GetIntDefault("TryLockTimes", 3);
    // 连接到zookeeper服务器，使用connection_watcher作为连接事件的回调函数
    zh = zookeeper_init(host.c_str(), connection_watcher, timout, NULL, NULL, 0);
    if (zh == NULL)
    {
        zkcpp_log_error_core(ZKCPP_LOG_EMERG, errno, "failed to connect zookeeper server");
        exit(-1);
    }
    sleep(3);
    const char *root_lock_path = config_instance->GetString("RootLockPath");
    create_node_sync(root_lock_path, "", 0);
    std::cout<<"zoo_state(zh): "<<zoo_state(zh)<<std::endl;
}

// 关闭zookeeper客户端
ZookeeperClient::~ZookeeperClient()
{
    // 关闭与zookeeper服务器的连接
    zookeeper_close(zh);
}

// 处理连接事件
void ZookeeperClient::connection_watcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx)
{
    if (type == ZOO_SESSION_EVENT)
    {
        if (state == ZOO_CONNECTED_STATE)
        {
            zkcpp_log_error_core(ZKCPP_LOG_INFO, 0, "connected to zookeeper server");
        }
        else if (state == ZOO_EXPIRED_SESSION_STATE)
        {
            zkcpp_log_error_core(ZKCPP_LOG_INFO, 0, "zookeeper session expired");
        }
    }
}

bool ZookeeperClient::create_node_sync(const char *path, const char *data, int statu)
{
    // 检查节点路径的有效性
    if (path == nullptr || strlen(path) >= 256)
    {
        zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "try to create a invalid node path %s", path);
        return false;
    }

    // 检查客户端连接状态
    if (zoo_state(zh) != ZOO_CONNECTED_STATE)
    {
        zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "not connected to ZooKeeper server");
        return false;
    }

    char buffer[256]; // 用于存储实际创建的节点路径
    int buffer_len = sizeof(buffer); // 用于存储节点路径的长度

    // 检查节点是否已存在
    int node_exists = zoo_exists(zh, path, 0, nullptr);
    if (node_exists == ZNONODE)
    {
        // 节点不存在则创建
        int ret = zoo_create(zh, path, data, strlen(data), &ZOO_OPEN_ACL_UNSAFE, statu, buffer, buffer_len);
        if (ret == ZOK)
        {
            zkcpp_log_error_core(ZKCPP_LOG_INFO, 0, "success to create node: %s", path);
            return true;
        }
        else
        {
            zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "failed to create node: %s", path);
            return false;
        }
    }
    else if (node_exists == ZOK)
    {
        // 节点已存在，您可以选择适当的处理方式
        // 例如，抛出错误、更新节点或返回成功
        zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "failed to create node %s ",path);
        return false; // 或者根据应用需求进行处理
    }
    else
    {
        zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "failed to create node: %s", path);
        return false;
    }
}

std::string ZookeeperClient::read_node_sync(const char *path)
{
    if (path == nullptr || strlen(path) >= 256)
    {
        zkcpp_log_error_core(ZKCPP_LOG_INFO, 0, "failed to read node %s: %s", path, "invalid node path");
        return ""; 
    }

    // 检查客户端连接状态
    if (zoo_state(zh) != ZOO_CONNECTED_STATE)
    {
        zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "not connected to ZooKeeper server");
        return "";
    }

    char buffer[256]; // 用于存储节点的数据
    int buffer_len = sizeof(buffer); // 用于存储数据的长度
    struct Stat stat; // 用于存储节点的元数据

    // 读取节点数据
    int ret = zoo_get(zh, path, 0, buffer, &buffer_len, &stat);
    if (ret == ZOK)
    {
        return std::string(buffer, buffer + buffer_len);
        // printf("Read node %s: %s\n", path, buffer);
    }
    else if (ret == ZNONODE)
    {
        // 节点不存在，您可以选择适当的处理方式
        zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "node does not exist: %s",path);
        return ""; // 或者根据应用需求返回一个特定的值
    }
    else
    {
        zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "failed to read node: %s",path);
        return ""; // 或者根据应用需求返回一个特定的值
    }
}

bool ZookeeperClient::update_node_sync(const char *path, const char *data)
{
    if (path == nullptr || strlen(path) >= 256)
    {
        zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "invalid node path: %s",path);
        return false; // 或者根据应用需求返回一个特定的值
    }

    // 检查客户端连接状态
    if (zoo_state(zh) != ZOO_CONNECTED_STATE)
    {
        zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "not connected to ZooKeeper server: %s",path);
        return false; // 或者根据应用需求返回一个特定的值
    }

    struct Stat stat; // 用于存储节点的元数据

    // 检查节点是否存在
    int node_exists = zoo_exists(zh, path, 0, nullptr);
    if (node_exists == ZOK)
    {
        // 更新节点数据
        int ret = zoo_set(zh, path, data, strlen(data), -1);
        if (ret == ZOK)
        {
            return true;
            // printf("Updated node %s successfully.\n", path);
        }
        else
        {
            zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "failed to update node: %s",path);
            return false;
        }
    }
    else if (node_exists == ZNONODE)
    {
        // 节点不存在，您可以选择适当的处理方式
        zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "node does not exist: %s",path);
        return false; // 或者根据应用需求返回一个特定的值
    }
    else
    {
        zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "failed to check node existence: %s",path);
        return false; // 或者根据应用需求返回一个特定的值
    }
}

bool ZookeeperClient::delete_node_sync(const char *path)
{
    // 检查节点路径的有效性
    if (path == nullptr || strlen(path) >= 256)
    {
        zkcpp_log_error_core(ZKCPP_LOG_INFO, 0, "invalid node path: %s",path);
        return false; // 或者根据应用需求返回一个特定的值
    }

    // 检查客户端连接状态
    if (zoo_state(zh) != ZOO_CONNECTED_STATE)
    {
        zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "not connected to ZooKeeper server: %s",path);
        return false; // 或者根据应用需求返回一个特定的值
    }

    // 检查节点是否存在
    int node_exists = zoo_exists(zh, path, 0, nullptr);
    if (node_exists == ZOK)
    {
        // 删除节点
        int ret = zoo_delete(zh, path, -1);
        if (ret == ZOK)
        {
            zkcpp_log_error_core(ZKCPP_LOG_INFO, 0, "success to delete node: %s",path);
            return true;
        }
        else
        {
            zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "faile to delete node: %s",path);
            return false;
        }
    }
    else if (node_exists == ZNONODE)
    {
        zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "node does not exist: %s",path);
        // 节点不存在，您可以选择适当的处理方式
        return false; // 或者根据应用需求返回一个特定的值
    }
    else
    {
        zkcpp_log_error_core(ZKCPP_LOG_INFO, errno, "failed to check node existence: %s",path);
        return false; // 或者根据应用需求返回一个特定的值
    }
}
