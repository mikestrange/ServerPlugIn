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
#include "world.h"
#include "clients.h"
#include "user.h"

class Clients;

class Client : public Packet, virtual public BufferInterface
{
private:
    int fd;
    int mark_fd;
private:
    Locked fd_lock;
public:
    User user;
public:
    Client();
    virtual ~Client();
public:
    void onConnect(int fd);
    void Disconnect();
    void SendPacket(const void* bytes, size_t length);
    
private:
    friend class Clients;
    void UnConnect();
public:
    //actions
};

#endif /* client_h */
