//
//  world.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef world_h
#define world_h

#include "base_server.h"
#include "world_session.h"
#include "global.h"

class PotHook;

class WorldServer : public BaseServer
{
    STATIC_CLASS(WorldServer);

public:
    virtual void OnRemove(SOCKET_T sockfd)override;
    
    virtual void OnRegister(SOCKET_T sockfd, SocketHandler* sock)override;
    
    virtual void OnProcessPacket(SOCKET_T sockfd, SocketHandler& packet)override;
};

#endif /* world_hpp */
