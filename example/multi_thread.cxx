#include <algorithm>
#include <iostream>
#include <string>
#include <string.h>
#include <list>
#include "zkcpp.h"
using namespace std;
            // cout << "==== tid: "<<std::this_thread::get_id()<<" get lock and sleep ====" << endl;
ZookeeperClient zkcpp;
class msgQueue
{
private:
    std::list<int> msgque;
    mutex msgmutx;
public:
    void msg_push()
    {
        cout << " ==== tid: " << std::this_thread::get_id() << " producer runing ====" << endl;
        for (size_t i = 0; i < 100; ++i)
        {
            zkcpp.lock();
            zkcpp.lock(); // 可重入锁
            msgque.push_back(i);
            cout << "msg push: " << i << endl;
            zkcpp.unlock();
        }
    }
    void msg_get()
    {
        cout << " ==== tid: " << std::this_thread::get_id() << " consumer runing ====" << endl;
        for(size_t i = 0; i < 130; ++i)
        {
            zkcpp.lock();
            if (!msgque.empty())
            {
                int msgrecv = msgque.front(); // 读取队列顶层消息
                msgque.pop_front();
                cout << "msg get: " << msgrecv << endl;
            }
            zkcpp.unlock();
        }
    }
};
int main()
{
    msgQueue msgQue;
    std::thread producer(&msgQueue::msg_push, &msgQue);
    std::thread consumer(&msgQueue::msg_get, &msgQue);

    producer.join();
    consumer.join();
    return 0;
}