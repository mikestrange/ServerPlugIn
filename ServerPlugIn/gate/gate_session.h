//
//  gate_session.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/19.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef gate_session_hpp
#define gate_session_hpp

#include "global.h"

#include "base_server.h"
#include "command.h"

#include "GateLaunch.h"


class GateSession : public ISessionProxy
{
     STATIC_CLASS(GateSession);
public:
    virtual void OnPacketHandle(SOCKET_T sockfd, SocketHandler& packet)override;
    
private:
    //大厅处理
    void HandleToGate(SocketHandler& packet);
    //通知世界服务器
    void HandleToWorld(SocketHandler& packet);
    //直接通知玩家
    void HandleToPlayer(SocketHandler& packet);
    
private:
    
};

#endif /* gate_session_hpp */
