# 基于C++11实现muduo网络库
![image](https://user-images.githubusercontent.com/54012489/184050141-e008fa03-6b1f-4546-9b8e-aeaab35bdca0.png)

### 1.组件

- 小组件

  - noncopyable

  - Logger

  - Timestamp

- Reactor

  - Channel

  - Poller

  - EpollPoller

- Thread

  - Thread

  - EventLoopThread

  - EventLoopThreadPoll

- Acceptor

  - Socket

  - Acceptor

- TcpConnection
  - Buffer

- TcpServer

### 2.安装方法

```shell
./autobuild.sh
```

### 3.Echo服务器

```shell
cd example
make clean
make
./testserver

telnet 127.0.0.1 8000
```

