#pragma once

#include <vector>
#include <string>
#include <algorithm>

// 缓冲区
// 有一个prepend、read、write三个标志
// perpend-read之间是一个头部的标志位，read-write是可读数据，write-末尾是可写数据。
class Buffer
{
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024; // 头部后大小

    explicit Buffer(size_t initialSize = kInitialSize)
        : buffer_(kCheapPrepend + initialSize), readerIndex_(kCheapPrepend), writerIndex_(kCheapPrepend)
    {
    }

    size_t readableBytes() const { return writerIndex_ - readerIndex_; }  // 可读的数据长度
    size_t writableBytes() const { return buffer_.size() - writerIndex_; } // 可写数据长度
    size_t prependableBytes() const { return readerIndex_; } // 头部长度

    /*返回缓冲区中，可读数据的起始地址*/
    const char *peek() const { return begin() + readerIndex_; }

    void retrieve(size_t len)
    {
        //缓冲区readindex 偏移
        if (len < readableBytes())
        {
            readerIndex_ += len; //说明只读了可读缓冲区的一部分(len)
        }
        else
        {
            retrieveAll();
        }
    }
    //缓冲区复位
    void retrieveAll()
    {
        readerIndex_ = writerIndex_ = kCheapPrepend;
    }

    // 读取所有数据
    std::string retrieveAllAsString()
    {
        return retrieveAsString(readableBytes());
    }

    //读取len长度数据
    std::string retrieveAsString(size_t len)
    {
        std::string result(peek(), len);
        retrieve(len);
        return result;
    }

    // 保证缓冲区有这么长的可写空间
    void ensureWriteableBytes(size_t len)
    {
        if (writableBytes() < len)
        {
            makeSpace(len); //扩容
        }
    }

    // 返回可写数据地址
    char *beginWrite()
    {
        return begin() + writerIndex_;
    }
    const char *beginWrite() const
    {
        return begin() + writerIndex_;
    }

    // 往缓冲区中添加数据
    void append(const char *data, size_t len)
    {
        ensureWriteableBytes(len);
        std::copy(data, data + len, beginWrite());
        writerIndex_ += len;
    }
    ssize_t readFd(int fd, int* saveErrno);
    ssize_t writeFd(int fd, int* saveErrno);

private:
    char *begin() { return &*buffer_.begin(); } // 缓冲区头数据的地址
    const char *begin() const { return &*buffer_.begin(); }

    //扩容函数
    void makeSpace(size_t len)
    {
        if (writableBytes() + prependableBytes() < len + kCheapPrepend)
        {
            buffer_.resize(writerIndex_ + len);
        }
        else
        {
            size_t readAble = readableBytes();
            std::copy(begin() + readerIndex_, begin() + writerIndex_, begin() + kCheapPrepend);
            readerIndex_ = kCheapPrepend;
            writerIndex_ = readerIndex_ + readAble;
        }
    }
    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writerIndex_;
};