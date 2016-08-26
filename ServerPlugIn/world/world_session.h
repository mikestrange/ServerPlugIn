//
//  world_session.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef world_session_h
#define world_session_h

#include "log.h"
#include "global.h"
#include "data_bank.h"
#include "string_utils.h"
#include "command.h"
#include "base_server.h"
#include "hook_manager.h"
//
#include "player_manager.h"
#include "world.h"
//body
#include "reg_body.h"
#include "login_body.h"

class WorldServer;
class PotHook;

class WorldSession : public ISessionProxy
{
    STATIC_CLASS(WorldSession);
    
public:
    virtual void OnPacketHandle(SOCKET_T sockfd, SocketHandler& packet)override;
    
private:
    //通知挂钩处理
    void HandleToHook(SocketHandler& packet);
    //世界处理
    void HandleToWorld(SocketHandler& packet);
    //通知玩家处理
    void HandleToPlayer(SocketHandler& packet);
    
private:
    //注册
    void OnUserRegistration(SocketHandler& packet);
    //登录
    void OnUserLogin(SocketHandler& packet);
    //
    void OnHookRegister(SocketHandler& packet);
    //
    void OnHookUnRegister(SocketHandler& packet);
    
    
};

#endif /* world_session_h */
