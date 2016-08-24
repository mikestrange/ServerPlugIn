//
//  conn_info.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/22.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "sock_node.h"

SockNode::SockNode()
:fd(0)
,isOn(FD_OFF){}

SockNode::SockNode(int fd, struct sockaddr_in &addr)
{
    OnConnect(fd, addr);
}

SockNode::~SockNode()
{
    setClose();
}

void SockNode::OnConnect(int fd, struct sockaddr_in &addr)
{
    this->fd = fd;
    this->addr = addr;
    this->isOn = FD_ON;
}

int SockNode::setClose()
{
    isOn = FD_OFF;
    return fd;
}

int SockNode::getFd()
{
    return fd;
}

bool SockNode::isClose()const
{
    return isOn == FD_OFF && isSetFd();
}

bool SockNode::isConnect()const
{
    return isOn == FD_ON && isSetFd();
}

bool SockNode::isSetFd()const
{
    return fd != FD_NULL;
}

void SockNode::Send(const void *bytes, size_t size)
{
    if(isConnect())
    {
        NET_SEND(fd, bytes, size);
    }
}

