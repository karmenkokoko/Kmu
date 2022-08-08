#include "Poller.h"
#include <vector>
#include <sys/epoll.h>
class Channel;

// 封装了epoll ，多路事件分发， 包含了指向channel的指针， 以及自己在内核事件表的fd
class EpollPoller : public Poller
{
public:
    EpollPoller(EventLoop* loop);
    ~EpollPoller();

    //重写IO接口 调用epoll_wait，并将发生事件的channel填写到形参active_channel中
    virtual Timestamp poll(int timeoutMs, ChannelList *activeChannels) override;
    //往channel_map中添加channel
    virtual void updateChannel(Channel *channel) override;
    //channel_map中删除channel
    virtual void removeChannel(Channel *channel) override;

private:
    static const int kInitEventListSize = 16;
    //填写活跃的连接
    void fillActiveChannels(int numEvents,ChannelList* activeChannels) const;
    //更新channel通道
    void update(int operation,Channel* channel);

    using EventList = std::vector<epoll_event>;
    int epollfd_;
    EventList events_;
};