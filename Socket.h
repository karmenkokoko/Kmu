#pragma once
#pragma once

#include "noncopyable.h"
class InetAddress;
// 对于socket bind lsiten accept 
class Socket : noncopyable
{
public:
    explicit Socket(int sockfd) : sockfd_(sockfd)
    {
    }
    ~Socket();

    int fd() const {return sockfd_;}
    void bindAddress(const InetAddress& localAddr);
    void listen();
    int accept(InetAddress* peerAddr);

    void shutdownWrite();

    void setTcpNodelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);
private:
    const int sockfd_;
};