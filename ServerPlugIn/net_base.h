//
//  net_base.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/5.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef net_base_h
#define net_base_h

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

#include "global.h"
#include "lock.h"

//服务器最大连接数目
#define MAX_SERVER 1024
//最大读取缓冲
#define MAX_BUFFER 1024

//socket回执方法
typedef void(*SCOKET_CALL)(int type, int fd, char* bytes, size_t length);

//socket回执事件
typedef enum _SOCKET_EVENT{
    SOCKET_ACCEPT = 1,
    SOCKET_READ,
    SOCKET_CLOSED,
    SOCKET_SELF_CLOSED,   //服务端被迫关闭
    SOCKET_CONNECT,//socket
}SOCKET_EVENT;

namespace net
{
    bool close_fd(int fd);
    
    int recv_fd(int fd, void* bytes, size_t len);
    
    bool send_fd(int fd, const void* bytes, size_t len);
}

//class netbase
class NetBase
{
private:
    int _maxfds;
private:
    int fd_list[MAX_SERVER];
    Locked fds_lock;
    
public:
    NetBase()
    :_maxfds(0)
    {
        init_fds();
    }
    
protected:
    virtual void maxfds(int max)
    {
        _maxfds = max;
        if(_maxfds < 0) _maxfds = 1; //不能少于1
        if(_maxfds > MAX_SERVER) _maxfds = MAX_SERVER;
    }
    
    virtual int maxfds()
    {
        return _maxfds;
    }
    
protected:
    virtual void init_fds()
    {
        AUTO_LOCK(&fds_lock);
        for(int i = 0; i < MAX_SERVER; i++)
        {
            fd_list[i] = 0;
        }
    }
    
    //添加一个套接字
    virtual bool add(int fd)
    {
        AUTO_LOCK(&fds_lock);
        for(int i = 0; i < _maxfds; i++)
        {
            if(fd_list[i] == 0)
            {
                fd_list[i] = fd;
                return true;
            }
        }
        return false;
    }
    
    //置空一个套接字 fd>0表示成功
    virtual int remove(int floor, fd_set* fdset)
    {
        AUTO_LOCK(&fds_lock);
        return CloseSocket(floor, fdset);
    }
    
    virtual int reset(fd_set* fdset)
    {
        AUTO_LOCK(&fds_lock);
        int max_fd = 0;
        for(int i = 0; i < _maxfds; i++)
        {
            int fd = fd_list[i];
            if(fd != 0)
            {
                FD_SET(fd, fdset);
                if(max_fd < fd) max_fd = fd;
            }
        }
        return max_fd;
    }
    
    virtual int isset(int floor, fd_set* fdset)
    {
        AUTO_LOCK(&fds_lock);
        int fd = fd_list[floor];
        if(fd != 0)
        {
            if(FD_ISSET(fd, fdset))
            {
                return fd;
            }
            return 0;
        }
        return false;
    }
    
    virtual bool shut(int fd, fd_set* fdset)
    {
        AUTO_LOCK(&fds_lock);
        for(int i = 0; i < _maxfds; i++)
        {
            if(fd_list[i] == fd)
            {
                return CloseSocket(i, fdset) > 0;
            }
        }
        return false;
    }
    
    virtual void clean(fd_set* fdset, SCOKET_CALL func)
    {
        for(int i = 0; i < _maxfds; i++)
        {
            int fd = remove(i, fdset);
            if(fd > 0 && i > 0)
            {
                func(SOCKET_CLOSED, fd, NULL, NULL);
            }
        }
    }
    
private:
    virtual int CloseSocket(int floor, fd_set* fdset)
    {
        int fd = fd_list[floor];
        if(fd != 0)
        {
            fd_list[floor] = 0;
            net::close_fd(fd);
            FD_CLR(fd, fdset);
        }
        return fd;
    }
    
public:
    virtual void toString()
    {
        trace("##server toString begin:");
        for(int i = 0; i < _maxfds; i++)
        {
            if(fd_list[i] != 0)
            {
                trace("on line fd = %d", fd_list[i]);
            }
        }
        trace("##server toString end");
    }
};

#endif /* net_base_hpp */
