//
//  GateServer.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/19.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef GateServer_h
#define GateServer_h

#include "global.h"
#include "base_server.h"
#include "gate_session.h"

class GateServer : public BaseServer
{
    STATIC_CLASS(GateServer);
public:
    virtual void OnRemove(SOCKET_T sockfd)override;
    
    virtual void OnRegister(SOCKET_T sockfd, SocketHandler* sock)override;
    
    virtual void OnProcessPacket(SOCKET_T sockfd, SocketHandler& packet)override;
};
#endif /* GateServer_h */
