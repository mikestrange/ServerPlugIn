//
//  net_base.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/5.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef net_base_h
#define net_base_h

#include "net.h"

//class netbase
class FdList
{
private:
    int _maxfds;
private:
    int fd_list[MAX_SERVER];
    
public:
    FdList()
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
        for(int i = 0; i < MAX_SERVER; i++)
        {
            fd_list[i] = 0;
        }
    }
    
    //添加一个套接字
    virtual bool add(int fd)
    {
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
        return CloseSocket(floor, fdset);
    }
    
    virtual int reset(fd_set* fdset)
    {
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
        for(int i = 1; i < _maxfds; i++)
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
            FD_CLR(fd, fdset);
            net::close_fd(fd);
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
