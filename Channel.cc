#include "Channel.h"
#include "EventLoop.h"
#include "Logger.h"
#include <sys/epoll.h>

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = 1;
const int Channel::kWriteEvent = 2;

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1), tied_(false)
{
}
Channel::~Channel() {}
/**
 * @brief  Channel的tie方法什么时候调用？
 *         一个TcpConnection新连接创建的时候
 *         TCPConnection => Channel
 * @param  obj:
 */
void Channel::tie(const std::shared_ptr<void> &obj)
{
    tie_ = obj;
    tied_ = true;
}

//在当前Channel所属的EventLoop中删掉Channel
void Channel::remove()
{
    loop_->removeChannel(this);
}

/**
 * 当改变Channel所表示的fd的events事件后，update负责在poller里面更改
 * fd相应的事件epoll_ctl
 */
void Channel::update()
{
    loop_->updateChannel(this);
}

// fd得到poller的通知后，处理事件
void Channel::handleEvent(Timestamp receiveTime)
{
    if (tied_) {
        // 通过调用std::weak_ptr类提供的lock()方法来获取管理所监测资源的shared_ptr对象
        std::shared_ptr<void> guard = tie_.lock();
        if(guard){
            handleEventWithGuard(receiveTime);
        }
    }

    else{
        handleEventWithGuard(receiveTime);
    }
}


//根据poller通知的channel所发生的具体事件，由channel负责调用具体的回调操作
void Channel::handleEventWithGuard(Timestamp receiveTime)
{
    LOG_INFO("channel handleEvent revents:%d\n", revents_);

    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) // 表示对应的文件描述符被挂断；
    {
        if (closeCallback_)
        {
            closeCallback_();
        }
    }

    if (revents_ & EPOLLERR)
    {
        if (errorCallback_)
        {
            errorCallback_();
        }
    }

    if (revents_ & (EPOLLIN | EPOLLPRI))  // 表示对应的文件描述符有紧急的数据可读 表示对应的文件描述符可以读
    {
        if (readCallback_)
        {
            readCallback_(receiveTime);
        }
    }

    if (revents_ & EPOLLOUT) // 表示对应的文件描述符可以写；
    {
        if (writeCallback_)
        {
            writeCallback_();
        }
    }
}
