//
//  client.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "client.h"



Client::Client()
{
    
}

Client::~Client()
{
    trace("~Client %d", mark_fd);
    Disconnect();
}

void Client::onConnect(int fd)
{
    AUTO_LOCK(&fd_lock);
    this->fd = fd;
    this->mark_fd = fd;
}

bool Client::isConnect()
{
    AUTO_LOCK(&fd_lock);
    return fd > 0;
}

void Client::Disconnect()
{
    AUTO_LOCK(&fd_lock);
    if(fd > 0)
    {
        trace("Disconnect :%d", fd);
        int _fd = fd;
        fd = 0;
    }
}

void Client::UnConnect()
{
    AUTO_LOCK(&fd_lock);
    if(fd > 0)
    {
        trace("UnConnect :%d", fd);
        fd = 0;
    }
}

void Client::SendPacket(const void *bytes, size_t length)
{
    AUTO_LOCK(&fd_lock);
    if(fd > 0){
        //NET_SEND(fd, bytes, length);
    }else{
        trace("this client is close");
    }
}