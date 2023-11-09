#include "zkcpp.h"
#include "zkcpp_log.h"
#include "zkcpp_macro.h"
void ZookeeperClient::create_completion(int rc, const char* name, const void* data) {
    if (rc == ZOK) {
        zkcpp_log_error_core(ZKCPP_LOG_INFO, 0, "success create node: %s", name);
    } else {
        zkcpp_log_error_core(ZKCPP_LOG_ERR, errno, "failed to create node: %s", name);
    }
}

void ZookeeperClient::create_node_async(const char* path, const char* data, int statu) {
    zoo_acreate(zh, path, data, strlen(data), &ZOO_OPEN_ACL_UNSAFE, statu, create_completion, nullptr);
}

void ZookeeperClient::read_completion(int rc, const char* value, int value_len, const struct Stat* stat, const void* data) {
    if (rc == ZOK) {
        std::string data_str(value, value + value_len);
        std::cout<<"===== read node info: "<<data_str<< " ====="<<std::endl;
    } else {
    }
}

void ZookeeperClient::read_node_async(const char* path) {
    zoo_aget(zh, path, 0, read_completion, nullptr);
}

void ZookeeperClient::update_completion(int rc, const struct Stat* stat, const void* data) {
    if (rc == ZOK) {
        zkcpp_log_error_core(ZKCPP_LOG_INFO, 0, "success update node async");
    } else {
        zkcpp_log_error_core(ZKCPP_LOG_ERR, errno, "failed updated node async");
    }
}

void ZookeeperClient::update_node_async(const char* path, const char* data) {
    zoo_aset(zh, path, data, strlen(data), -1, update_completion, nullptr);
}

void ZookeeperClient::delete_completion(int rc, const void* data) {
    if (rc == ZOK) {
        zkcpp_log_error_core(ZKCPP_LOG_INFO, 0, "success update node async");
    } else {
        zkcpp_log_error_core(ZKCPP_LOG_ERR, errno, "failed update async: %s");
    }
}

void ZookeeperClient::delete_node_async(const char* path) {
    zoo_adelete(zh, path, -1, delete_completion, nullptr);
}