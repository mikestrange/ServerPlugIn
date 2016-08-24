//
//  socket.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/22.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "socket.h"


//class netsocket
bool NetSocket::Connect(const char* host, int port)
{
    if(isConnect())
    {
        return false;
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        Log::warn("socket af_inet error: %s:%d", host, port);
    }else{
        struct sockaddr_in sock_addr;
        memset(&sock_addr, 0, sizeof(sock_addr));
        sock_addr.sin_family        = AF_INET;
        sock_addr.sin_addr.s_addr   = inet_addr(host);
        sock_addr.sin_port          = htons(port);
        //
        if (::connect(sockfd, (struct sockaddr *)&sock_addr, sizeof(struct sockaddr)) == -1)
        {
            Log::warn("socket connect error: %s:%d %d", host, port, sockfd);
        }else{
            Log::info("socket connect ok: %s:%d %d", host, port, sockfd);
            return true;
        }
    }
    sockfd = 0;
    return false;
}

int NetSocket::PollAttemper()
{
    //
    char bytes[MAX_BUFFER];
    for(;;)
    {
        int ret = NET_RECV(sockfd, bytes, MAX_BUFFER);
        if (ret > 0)
        {
            trace("socket read = %d", ret);
            OnRead(bytes, ret);
            memset(bytes, 0, ret);
        }else if(ret == 0){
            Log::warn("##socket is close");
            break;
        }else{
            if (errno == EINTR || errno == EAGAIN){
                
            }else{
                Log::warn("##socket read error: %d",ret);
                break;
            }
        }
    }
    Disconnect();
    OnClose();
    return -1;
}

void NetSocket::OnRead(char *bytes, size_t size)
{
    
}

void NetSocket::OnClose(){
    
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