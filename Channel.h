#pragma once
#include "noncopyable.h"
#include "Timestamp.h"
#include <functional>
#include <memory>
// 封装了sockfd及其所感兴趣的事件，
// 还有发生事件所要调用的回调函数。

class EventLoop;
/**
 * @brief
 */
class Channel : noncopyable
{
public:
    using EventCallback = std::function<void()>; // 指定变量别名
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(EventLoop *loop, int fd);
    ~Channel();

    // fd得到poller的通知后，处理事件
    void handleEvent(Timestamp receiveTime);

    //设置回调函数
    void setReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }
    void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }

    //防止当Channel被手动remove掉,Channel还在执行回调
    void tie(const std::shared_ptr<void> &);

    // 得到socket套接字
    int fd() const { return fd_; }
    // 得到感兴趣的事件
    int events() const { return events_; }
    // 设置真正发生的事件， poller监听到，然后设置real_event
    int set_revents(int revt) { revents_ = revt; }

    //设置fd相应的事件状态
    void enableReading()
    {
        events_ |= kReadEvent;
        update();
    }
    void disableReading()
    {
        events_ &= ~kReadEvent;
        update();
    }
    void enableWriting()
    {
        events_ |= kWriteEvent;
        update();
    }
    void disableWriting()
    {
        events_ &= kWriteEvent;
        update();
    }
    void disableAll()
    {
        events_ = kNoneEvent;
        update();
    }

    //返回fd当前的事件状态
    bool isNoneEvent() const { return events_ == kNoneEvent; }
    bool isWriting() const { return events_ & kWriteEvent; }
    bool isReading() const { return events_ & kReadEvent; }


    int index() const { return index_; }
    void set_index(int idx) { index_ = idx; }

    //返回哪个loop拥有了当前Channel
    EventLoop *ownerLoop() const { return loop_; }
    //在channel所属的eventloop中删除自己
    void remove();

private:
    //与poller更新fd所感兴趣事件
    void update();
    //根据发生的具体事件调用相应的回调操作
    void handleEventWithGuard(Timestamp receiveTime);

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop *loop_; //事件循环
    const int fd_;    // poller监听的对象
    int events_;      //注册fd感兴趣的事件
    int revents_;     // poller返回的具体的发生的事件
    int index_;

    std::weak_ptr<void> tie_; //观察当前channel的存在状态
    bool tied_; // 判断tie_是否绑定过

    //负责具体的回调操作
    //发生事件所要调用的具体事件的回调操作
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};