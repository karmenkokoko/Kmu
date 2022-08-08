#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
class InetAddress
{
public:
// 必须要显示地调用构造函数
    explicit InetAddress(uint16_t port = 0, std::string ip = "127.0.0.1");
    explicit InetAddress(const sockaddr_in& addr):addr_(addr){}

    std::string toIp() const;
    std::string toIpPort() const;
    uint16_t toPort() const;

    const sockaddr_in* getSockAddr() const {return &addr_;}
    void setSockAddr(const sockaddr_in& addr) {addr_ = addr;}
    ~InetAddress() = default; // 默认析构函数
private:
    sockaddr_in addr_;
};