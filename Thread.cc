#include "Thread.h"
#include "CurrentThread.h"
#include <atomic>
#include <thread>
#include <semaphore.h>
std::atomic_int Thread::numCreated_(0);

Thread::Thread(ThreadFunc func, const std::string &name)
    :started_(false)
    ,joined_(false)
    ,tid_(0)
    ,func_(std::move(func))
    ,name_(name)
{
    setDefaultName();
}

Thread::~Thread(){
    if(started_ && !joined_) {
        thread_->detach(); //分离线程
    }
}

void Thread::join(){
    joined_ = true;
    thread_->join();
}

void Thread::start(){
    started_ = true;
    sem_t sem;
    sem_init(&sem, false, 0);
    /*开启线程*/
    // lambda表达式初始化智能指针控制线程的生存周期
    thread_ = std::shared_ptr<std::thread>(new std::thread([&](){
        tid_ = CurrentThread::tid();
        sem_post(&sem);
        func_(); //开启一个线程，用于执行fun方法
    }));
    /*这里必须等待获取tid*/
    sem_wait(&sem);
}


void Thread::setDefaultName(){
    int num = ++numCreated_;
    if(name_.empty()){
        char buf[32] = {0};
        snprintf(buf, sizeof buf, "Thread%d",num);
        name_ = buf;
    }
}

