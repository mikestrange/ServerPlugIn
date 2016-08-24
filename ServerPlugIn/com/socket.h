//
//  socket.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/22.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef socket_hpp
#define socket_hpp

#include "net.h"

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
    
    virtual int PollAttemper();
    
    virtual void Disconnect();
    
    virtual bool isConnect()const;
    
    virtual void Send(const void* bytes, size_t length);
    
protected:
    virtual void OnRead(char* bytes, size_t size);
    virtual void OnClose();
};

#endif /* socket_hpp */
