//
//  client.h
//  ServerPlugIn
//  用来回执用户
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef client_h
#define client_h

#include "lock.h"
#include "world.h"
#include "clients.h"
#include "user.h"
#include "packet_buffer.h"

class Clients;

class Client
{
private:
    int fd;
    int mark_fd;
private:
    Locked fd_lock;
public:
    PacketBuffer packet;
public:
    User user;
    //
public:
    Client();
    virtual ~Client();
public:
    void onConnect(int fd);
    void Disconnect();
    void SendPacket(PacketBuffer& buf);
    void onDelete();
private:
    friend class Clients;
    void UnConnect();
public:
    //actions
};

#endif /* client_h */
