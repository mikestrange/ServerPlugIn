//
//  socket.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "socket.h"

NetSocket::NetSocket()
:host(NULL)
,port(0)
,sockfd(0)
{
    
}

NetSocket::~NetSocket()
{
    closed();
}

bool NetSocket::isConnected()const
{
    return sockfd > 0;
}

bool NetSocket::connect(const char *host, int port)
{
    if(isConnected())
    {
        return true;
    }
    this->host = host;
    this->port = port;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        trace("socket af_inet error: %s:%d", host, port);
        sockfd = 0;
    }else{
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

void NetSocket::poll(SCOKET_CALL func)
{
    char bytes[MAX_BUFFER];
    while(isConnected())
    {
        int ret = net::recv_fd(sockfd, bytes, MAX_BUFFER);
        if (ret > 0)
        {
            trace("socket read = %d", ret);
            func(SOCKET_READ, sockfd, bytes, ret);
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
    closed();
    //通知
    func(SOCKET_SELF_CLOSED, NULL, NULL, NULL);
}

void NetSocket::closed()
{
    if(sockfd > 0)
    {
        int _sockfd = sockfd;
        sockfd = 0;
        net::close_fd(_sockfd);
    }else{
        trace("close error: this socket is closed");
    }
}

void NetSocket::SendPacket(const void* bytes, size_t length)
{
    if(isConnected())
    {
        net::send_fd(sockfd, bytes, length);
    }else{
        trace("SendPacket error: this socket is closed");
    }
}
