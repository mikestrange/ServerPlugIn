//
//  net.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/9.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "net.h"

bool NET_CLOSE(int fd)
{
    if(fd > 0)
    {
        int ret = close(fd);
        trace("close fd = %d ret = %d", fd, ret);
        return ret == 0;
    }
    trace("close fd < 0(%d)", fd);
    return false;
}

int NET_RECV(int fd, void* bytes, size_t len)
{
    return (int)::recv(fd, bytes, len, 0);
}

bool NET_SEND(int fd, const void* bytes, size_t len)
{
    if(fd > 0){
        size_t ret = send(fd, bytes, len, 0);
        trace("send to fd = %d ret = %ld, len = %ld", fd, ret, len);
        return (int)ret >= 0;
    }else{
        trace("send error: (fd <= 0) fd = %d",fd);
    }
    return false;
}

//class server
bool NetServer::Open(int port)
{
    if(isOpen()){
        return true;
    }
    serverId = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    //绑定端口
    if(::bind(serverId, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0 &&
       listen(serverId, DEF_LISTEN_COUNT) == 0)
    {
        trace("open server ok port = %d serid = %d", port, serverId);
        return true;
    }else{
        trace("open server error port = %d", port);
    }
    serverId = 0;
    return false;
}

int NetServer::PollAttemper(SCOKET_CALL perform)
{
    if(!isOpen())
    {
        return -1;
    }
    //FdList fd_list;
    fd_list.CLEAN_FDS();
    //set
    fd_set readfds;
    //客户端
    struct sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    //清理
    FD_ZERO(&readfds);
    //添加主体
    fd_list.NEW_FD(serverId, &readfds, server_addr);
    //缓冲区
    char bytes[MAX_BUFFER];
    //5秒超时
    struct timeval timeout;
    //select
    for(;;)
    {
        FD_ZERO(&readfds);
        //超时时间
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        //最大的套接字(重新设置)
        int max_fd = fd_list.RESET_FDS(&readfds);
        if(max_fd < serverId) max_fd = serverId;
        //监听
        int result = select(max_fd + 1, &readfds, NULL, NULL, &timeout);
        //端口错误
        if(result < 0)
        {
            if (errno == EINTR || errno == EAGAIN) continue;
            //
            trace("--select error:%d %d--", result, errno);
            break;
        }else if(result == 0){
            trace("--select timeout--");
        }else{
            for(int i = 0; i < fd_list.maxfds(); i++)
            {
                int fd = fd_list.ISON_FD(i, &readfds);
                if(fd > 0)
                {
                    if(fd == serverId)
                    {
                        int client_fd = accept(serverId, (struct sockaddr *)&client_addr, (socklen_t *)&client_len);
                        if(client_fd > 0 && fd_list.NEW_FD(client_fd, &readfds, client_addr))
                        {
                            trace("accept client: %d", client_fd);
                            perform(SOCKET_ACCEPT, client_fd, NULL, NULL);
                        }else{
                            trace("--accept error : %d--", client_fd);
                            NET_CLOSE(client_fd);
                        }
                    }else{
                        int ret = NET_RECV(fd, bytes, MAX_BUFFER);
                        if(ret > 0){
                            trace("read fd = %d len = %d", fd, ret);
                            perform(SOCKET_READ, fd, bytes, ret);
                            memset(bytes, 0, ret);
                        }else{
                            if (errno == EINTR || errno == EAGAIN) continue;
                            trace("错误和客户端de关闭 %d",fd);
                            fd_list.REMOVE_FD(i, &readfds);
                            perform(SOCKET_CLOSED, fd, NULL, NULL);
                        }
                    }
                }else{
                    fd_list.REMOVE_FD(i, &readfds);
                }
            }
        }
    };
    //---
    fd_list.CLEAN_FDS();
    //
    Shut();
    //通知
    perform(SOCKET_SELF_CLOSED, serverId, NULL, NULL);
    return -1;
}

bool NetServer::isOpen()const
{
    return serverId > 0;
}

void NetServer::Shut()
{
    if(isOpen())
    {
        NET_CLOSE(serverId);
    }
}

void NetServer::Shut(int fd)
{
    fd_list.PUSH_CLOSE(fd);
}

//class netsocket
bool NetSocket::Connect(const char* host, int port)
{
    if(isConnect()){
        return true;
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        trace("socket af_inet error: %s:%d", host, port);
    }else{
        struct sockaddr_in sock_addr;
        memset(&sock_addr, 0, sizeof(sock_addr));
        sock_addr.sin_family        = AF_INET;
        sock_addr.sin_addr.s_addr   = inet_addr(host);
        sock_addr.sin_port          = htons(port);
        //
        if (::connect(sockfd, (struct sockaddr *)&sock_addr, sizeof(struct sockaddr)) == -1)
        {
            trace("socket connect error: %s:%d %d", host, port, sockfd);
        }else{
            trace("socket connect ok: %s:%d %d", host, port, sockfd);
            return true;
        }
    }
    sockfd = 0;
    return false;
}

int NetSocket::PollAttemper(SCOKET_CALL perform)
{
    //
    char bytes[MAX_BUFFER];
    for(;;)
    {
        int ret = NET_RECV(sockfd, bytes, MAX_BUFFER);
        if (ret > 0)
        {
            trace("socket read = %d", ret);
            perform(SOCKET_READ, sockfd, bytes, ret);
            memset(bytes, 0, ret);
        }else if(ret == 0){
            trace("##socket is close");
            break;
        }else{
            if (errno == EINTR || errno == EAGAIN){
                
            }else{
                trace("##socket read error: %d",ret);
                break;
            }
        }
    }
    Disconnect();
    perform(SOCKET_SELF_CLOSED, sockfd, NULL, NULL);
    return -1;
}

void NetSocket::Disconnect()
{
    if(sockfd > 0)
    {
        int fd = sockfd;
        sockfd = 0;
        NET_CLOSE(fd);
    }
}

void NetSocket::Send(const void *bytes, size_t length)
{
    if(isConnect())
    {
        NET_SEND(sockfd, bytes, length);
    }
}

bool NetSocket::isConnect()const
{
    return sockfd > 0;
}

//class fd list

FdList::FdList()
{
    INIT_FDS();
}

void FdList::INIT_FDS()
{
    for(int i = 0; i < MAX_CONNECTS; i++)
    {
        fd_list[i] = {-1, 0};   // connectors.
    }
}

int FdList::maxfds()
{
    return MAX_CONNECTS;
}

bool FdList::NEW_FD(int fd, fd_set* fdset, struct sockaddr_in& client_address)
{
    for(int i = 0; i < maxfds(); i++)
    {
        if(fd_list[i].isOn)
        {
            fd_list[i] = {fd, 1, client_address};
            FD_SET(fd, fdset);
            return true;
        }
    }
    return false;
}

bool FdList::REMOVE_FD(int p, fd_set* fdset)
{
    if(fd_list[p].isOn||fd_list[p].fd <= 0) return false;
    //关闭socket
    int fd = fd_list[p].fd;
    fd_list[p].isOn = 0;
    fd_list[p].fd = -1;
    FD_CLR(fd, fdset);
    NET_CLOSE(fd);
    return true;
}

int FdList::RESET_FDS(fd_set* fdset)
{
    int max_fd = 0;
    for(int i = 0; i < maxfds(); i++)
    {
        if(fd_list[i].isOn)
        {
            int fd = fd_list[i].fd;
            FD_SET(fd, fdset);
            if(max_fd < fd) max_fd = fd;
        }
    }
    return max_fd;
}

int FdList::ISON_FD(int p, fd_set* fdset)
{
    int fd = fd_list[p].fd;
    if(fd_list[p].isOn && fd > 0 && FD_ISSET(fd, fdset))
    {
        return fd;
    }
    return -1;
}

//关闭而已
void FdList::PUSH_CLOSE(int fd)
{
    for(int i = 0; i < maxfds(); i++)
    {
        if(fd > 0 && fd == fd_list[i].fd)
        {
            fd_list[i].isOn = 0;
            break;
        }
    }
}

void FdList::CLEAN_FDS()
{
    for(int i = 0; i < MAX_CONNECTS; i++)
    {
        fd_list[i].isOn = 0;
        fd_list[i].fd = -1;
    }
}

void FdList::toString()
{
    trace("##server toString begin:");
    for(int i = 0; i < maxfds(); i++)
    {
        if(fd_list[i].isOn)
        {
            trace("on line fd = %d", fd_list[i].fd);
        }
    }
    trace("##server toString end");
}
