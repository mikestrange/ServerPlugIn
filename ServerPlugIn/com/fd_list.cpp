//
//  fd_list.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/22.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "fd_list.h"

//class fd list
FdList::FdList()
{
    INIT_FDS();
}

void FdList::INIT_FDS()
{
    for(int i = 0; i < MAX_CONNECTS; i++)
    {
        fd_list[i] = NULL;   // connectors.
    }
}

int FdList::maxfds()
{
    return MAX_CONNECTS;
}

SockNode* FdList::NewAccept(int fd, fd_set* fdset, struct sockaddr_in& client_addr)
{
    for(int i = 0; i < maxfds(); i++)
    {
        auto node = getConnInfo(i);
        if(!node)
        {
            node = new SockNode(fd,client_addr);
            fd_list[i] = node;
            return node;
        }
    }
    return NULL;
}

SockNode* FdList::CloseConn(int p, fd_set* fdset)
{
    auto node = SetNull(p);
    if(node)
    {
        int fd = node->setClose();
        FD_CLR(fd, fdset);
        NET_CLOSE(fd);
    }
    return node;
}

int FdList::ResetConns(fd_set* fdset)
{
    int max_fd = 0;
    for(int i = 0; i < maxfds(); i++)
    {
        auto conn = getConnInfo(i);
        if(conn)
        {
            int fd = conn->getFd();
            FD_SET(fd, fdset);
            if(max_fd < fd) max_fd = fd;
        }
    }
    return max_fd;
}

//服务器不由它来关闭
void FdList::Clean()
{
    for(int i = 0; i < MAX_CONNECTS; i++)
    {
        auto conn = SetNull(i);
        if(conn)
        {
            NET_CLOSE(conn->setClose());
            //发送处理
        }
    }
}

SockNode* FdList::getConnInfo(int index)
{
    return fd_list[index];
}

SockNode* FdList::SetNull(int index)
{
    auto node = getConnInfo(index);
    fd_list[index] = NULL;
    return node;
}


void FdList::toString()
{
    Log::info("##server toString begin:");
    for(int i = 0; i < maxfds(); i++)
    {
        auto node = getConnInfo(i);
        if(node && node->isConnect())
        {
            Log::info("on line fd = %d", node->getFd());
        }
    }
    Log::info("##server toString end");
}