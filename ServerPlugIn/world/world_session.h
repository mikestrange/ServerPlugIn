//
//  world_session.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef world_session_h
#define world_session_h

#include "global.h"
#include "log.h"
#include "data_bank.h"
#include "string_utils.h"

#include "command.h"

#include "packet_buffer.h"
#include "player_manager.h"
#include "network.h"

#include "pothook.h"
//body
#include "reg_body.h"
#include "login_body.h"

class WorldServer;
class PotHook;

class WorldSession
{
    STATIC_CLASS(WorldSession);
public:
    
    void HandlePacket(SocketHandler& packet);
private:
    void HandleToWorld(SocketHandler& packet);
    //用户注册
    void OnUserRegistration(SocketHandler& packet);
    //用户登录
    void OnUserLogin(SocketHandler& packet);
    //
    void OnHookRegister(SocketHandler& packet);
    //
    void OnHookUnRegister(SocketHandler& packet);
    
    
public:
    
};

#endif /* world_session_h */
