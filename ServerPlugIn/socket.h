//
//  socket.h
//  ServerPlugIn
//  服务器连接器
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef socket_h
#define socket_h

#include "server.h"

class NetSocket
{
private:
    const char* host;
    int port;
    int sockfd;//(-1/0)表示无连接
private:
    struct sockaddr_in sock_addr;
public:
    NetSocket();
    
    virtual ~NetSocket();
    
    virtual bool connect(const char* host,int port);
    
    virtual void poll(SCOKET_CALL func);
    
    virtual bool isConnected()const;
    
    virtual void closed();
    
    virtual void SendPacket(const void* bytes, size_t length);
};

#endif /* socket_h */
