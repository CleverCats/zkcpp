#ifndef __ZKCPP_H__
#define __ZKCPP_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unordered_map>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <mutex>
#include <iostream>
#include <zookeeper/zookeeper.h>

struct contextinfo
{
    std::mutex node_lock;
    std::atomic<bool> notified{false};
    std::condition_variable node_check;
};

class ZookeeperClient
{
private:
    // zookeeper的句柄
    zhandle_t *zh;
    // lock_thread_id lock
    std::mutex flash_threads_queue;
    // 线程局部环境
    // 存储每个线程获取的锁-可重入锁机制
    std::unordered_map<std::thread::id, std::string> lock_thread_id;
    // 处理连接事件
    static void connection_watcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx);
    int try_times = 3;
public:
    // 初始化zookeeper客户端
    ZookeeperClient(int timout = 0, std::string host = "");

    // 关闭zookeeper客户端
    ~ZookeeperClient();

    /**
     * brief 创建zk节点默认持久化节点
     * param statu: ZOO_EPHEMERAL 临时节点
     *              ZOO_SEQUENCE 顺序节点
     *              ZOO_EPHEMERAL | ZOO_SEQUENCE 临时顺序节点
    */
    bool create_node_sync(const char *path, const char *data, int statu = 0);

    // 读取节点
    std::string read_node_sync(const char *path);

    // 更新节点
    bool update_node_sync(const char *path, const char *data);

    // 删除节点
    bool delete_node_sync(const char *path);

    // 获取分布式锁
    void lock(std::string lockpath = "/lock");

    // 解锁
    void unlock(std::string lockpath = "/lock");
    
    // 异步CRUD
    void create_node_async(const char* path, const char* data, int statu);
    void read_node_async(const char* path);
    void update_node_async(const char* path, const char* data);
    void delete_node_async(const char* path);

    // 异步回调
    static void create_completion(int rc, const char* name, const void* data);
    static void read_completion(int rc, const char* value, int value_len, const struct Stat* stat, const void* data);
    static void update_completion(int rc, const struct Stat* stat, const void* data);
    static void delete_completion(int rc, const void* data);

private:
    // 处理锁事件
    static void lock_watcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx);

    // 获取加锁条件
    bool try_lock(std::thread::id tid, std::string node, std::string lockpath = "/lock");
};
#endif