//
//  net.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/9.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef net_h
#define net_h

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include "arpa/inet.h"

#include <errno.h>
#include <fcntl.h>
#include <err.h>

#include <map>

#include "global.h"
#include "time_utils.h"

class FdList;
class NetServer;
class NetSocket;

//最大读取缓冲
#define MAX_BUFFER 1024

//socket回执方法
typedef void(*SCOKET_CALL)(int type, int fd, char* bytes, size_t length);

//socket回执事件
typedef enum _SOCKET_EVENT
{
    SOCKET_ACCEPT = 1,
    SOCKET_READ,
    SOCKET_CLOSED,        //被动关闭(客户端)
    SOCKET_SELF_CLOSED,   //self close
}SOCKET_EVENT;


//公用
bool NET_CLOSE(int fd);

int NET_RECV(int fd, void* bytes, size_t len);

bool NET_SEND(int fd, const void* bytes, size_t len);


//服务器最大连接数目
#define MAX_CONNECTS 1024

typedef struct comefd
{
    int fd;
    int isOn;//状态，不代表存在
    struct sockaddr_in addr;
}comefd;

class FdList
{
private:
    comefd fd_list[MAX_CONNECTS];
public:
    FdList();
private:
    void INIT_FDS();
public:
    virtual void CLEAN_FDS();
    
    virtual int maxfds();
    
    virtual bool NEW_FD(int fd, fd_set* fdset, struct sockaddr_in& client_address);
    
    virtual bool REMOVE_FD(int p, fd_set* fdset);
    
    virtual void CLOSED_FD(int p, fd_set* fdset);
    
    virtual int RESET_FDS(fd_set* fdset);
    
    virtual int GET_FD(int p);
public:
    //外界调用(推送关闭)
    virtual void PUSH_CLOSE(int fd);
    
    virtual void toString();
};

//服务器

//最大listen处理
#define DEF_LISTEN_COUNT 5

class NetServer
{
private:
    int serverId;
    FdList fd_list;
    struct sockaddr_in server_addr;
public:
    NetServer()
    :serverId(0){}
    
    virtual ~NetServer()
    {
        Shut();
    }
    
    virtual bool Open(int port);
    
    virtual bool isOpen()const;
    
    virtual int PollAttemper(SCOKET_CALL perform);
    
    virtual void Shut();
    
    virtual void Shut(int fd);
    
    virtual void toString();
};


//客户端
class NetSocket
{
private:
    int sockfd;
    struct sockaddr_in server_addr;
public:
    NetSocket()
    :sockfd(0)
    {}
    
    virtual ~NetSocket()
    {
        Disconnect();
    }
    
    virtual bool Connect(const char* host, int port);
    
    virtual int PollAttemper(SCOKET_CALL perform);
    
    virtual void Disconnect();
    
    virtual bool isConnect()const;
    
    virtual void Send(const void* bytes, size_t length);
};

#endif /* net_hpp */
