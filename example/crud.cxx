#include <iostream>
#include <assert.h>
#include "zkcpp.h"

int main(int argc, char const *argv[])
{
    ZookeeperClient zk;
    bool res = false;
    res = zk.create_node_sync("/node1", "node1服务注册", 0);
    zk.create_node_async("/node2", "node2服务注册", 0);
    sleep(5);
    assert(res);
    std::cout<<"===== creat node successfully ====="<<std::endl;

    std::string zstr = zk.read_node_sync("/node1");
    zk.read_node_async("/node2");
    std::cout<<"===== read node info: "<<zstr<<" ====="<<std::endl;

    res = zk.update_node_sync("/node1", "node1 服务更新");
    zk.update_node_async("/node2", "node2 服务异步更新");
    sleep(5);
    assert(res);
    zstr = zk.read_node_sync("/node1");
    zk.read_node_async("/node2");
    std::cout<<"===== read node info: "<<zstr<<" ====="<<std::endl;
    sleep(5);
    assert(res);
    std::cout<<"===== update node successfully ====="<<std::endl;

    res = zk.delete_node_sync("/node1");
    zk.delete_node_async("/node2");
    assert(res);
    std::cout<<"===== delete node1 successfully ====="<<std::endl;
    std::cout<<"===== delete node2 successfully ====="<<std::endl;
    
    sleep(5);
    return 0;
}
