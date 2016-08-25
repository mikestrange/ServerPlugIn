//
//  socket_handler.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/25.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "socket_handler.h"


SocketHandler::SocketHandler()
{
    setHandler(0);
}

SocketHandler::SocketHandler(SOCKET_T sockt_fd)
{
    setHandler(sockt_fd);
}

SocketHandler::SocketHandler(int sock_id, SOCKET_T sockt_fd)
{
    setHandler(sockt_fd, sock_id);
}

SocketHandler::~SocketHandler()
{
    Disconnect();
}


void SocketHandler::setHandler(SOCKET_T sockt_fd, int sock_id)
{
    m_sock_fd = sockt_fd;
    m_sock_id = sock_id;
    is_connect = sockt_fd > 0;
}

void SocketHandler::Disconnect()
{
    is_connect = false;
}

bool SocketHandler::isConnect()const
{
    return is_connect;
}

int SocketHandler::handle_read(void* bytes, size_t size)
{
    int ret = Network::NET_RECV(m_sock_fd, bytes, size);
    if (ret > 0)
    {
        return ret;
    }else if(ret == 0){
        return -1;
    }else if(ret == -1){
        if (errno == EINTR || errno == EAGAIN) return 0;
    }
    return -1;
}

void SocketHandler::OnTimeout()
{
    
}

int SocketHandler::GetSocketId()
{
    return m_sock_id;
}

SOCKET_T SocketHandler::GetSocketFd()
{
    return m_sock_fd;
}

int SocketHandler::SendPacket(const void* bytes, size_t size)
{
    if(is_connect)
    {
        return Network::NET_SEND(m_sock_fd, bytes, size);
    }
    return -1;
}

int SocketHandler::SendPacket(PacketBuffer& packet)
{
    return SendPacket(&packet[0], packet.wsize());
}