//
//  client.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef client_h
#define client_h

#include "packet.h"
#include "lock.h"
#include "server.h"
#include "world.h"

class Client : public Packet, virtual public BufferInterface
{
private:
    int fd;
    int mark_fd;
private:
    Locked fd_lock;
public:
    Client();
    virtual ~Client();
public:
    void onConnect(int fd);
    bool isConnect();
    void Disconnect();
    void SendPacket(const void* bytes, size_t length);
    
public:
    //actions
};

#endif /* client_h */
