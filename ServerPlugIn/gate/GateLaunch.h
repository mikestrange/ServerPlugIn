//
//  GateLaunch.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/19.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef GateLaunch_h
#define GateLaunch_h

#include "global.h"
#include "base_socket.h"

#include "gate_proxy.h"
#include "command.h"

class GateLaunch : public BaseSocket
{
    STATIC_CLASS(GateLaunch);
public:
    GateLaunch();
    virtual ~GateLaunch();
public:
    virtual void OnConnect()override;
    virtual void OnClose()override;
    virtual void OnProcessPacket(SOCKET_T sockfd, PacketBuffer& packet)override;
    
private:
    GateProxy* proxy;
public:
    GateProxy* getProxy()const;
};

#endif /* GateLaunch_h */
