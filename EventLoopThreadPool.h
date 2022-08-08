#pragma once

#include "noncopyable.h"
#include <functional>
#include <string>
#include <vector>
#include <memory>
class EventLoop;
class EventLoopThread;
// subreactor池， thread_nums有相应数量sub_reactor
class EventLoopThreadPool : noncopyable
{
public:
    using ThreadInitCallback = std::function<void(EventLoop *)>;
    EventLoopThreadPool(EventLoop *baseLoop, const std::string &name = std::string());
    ~EventLoopThreadPool();

    void setThreadNum(int numThreads) { numThread_ = numThreads; }
    void start(const ThreadInitCallback& cb = ThreadInitCallback());

    /*如果工作在多线程中，baseLoop默认以轮询的方式分配channel给subloop*/
    EventLoop* getNextLoop();
    std::vector<EventLoop*> getAllLoops();
    
    bool started() const {return started_;}
    const std::string name() const {return name_;}

private:
    EventLoop *baseLoop_;
    std::string name_;
    bool started_;
    int numThread_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_; // 所有sub_reactor
    std::vector<EventLoop *> loops_; // 每个线程的loop事件循环
};