#include "zkcpp.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    ZookeeperClient zkcpp;
    int k = 0;
    while (true)
    {
        zkcpp.lock();
        std::cout <<++k <<" ======== c_2: get lock =======" << std::endl;
        sleep(10);
        zkcpp.unlock();
    }
    return 0;
}
