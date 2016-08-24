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
#include "data_bank.h"
#include "string_utils.h"

#include "command.h"

#include "packet_buffer.h"

#include "client.h"
#include "player_manager.h"
#include "pothook.h"

//body
#include "reg_body.h"
#include "login_body.h"

class Client;
class WorldServer;

class WorldSession
{
public:
    WorldSession();
    
    virtual ~WorldSession();
    
    void HandlePacket(Client* client);
private:
    void HandleToWorld(Client* client);
    //用户注册
    void OnUserRegistration(Client* client);
    //用户登录
    void OnUserLogin(Client* client);
    //
    void OnHookRegister(Client* client);
    //
    void OnHookUnRegister(Client* client);
    
    
public:
    
};

#endif /* world_session_h */
