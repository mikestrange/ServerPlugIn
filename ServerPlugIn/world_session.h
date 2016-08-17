//
//  world_session.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef world_session_h
#define world_session_h

#include "command.h"
#include "packet_buffer.h"
#include "client.h"

#include "log.h"
#include "string_utils.h"
#include "data_bank.h"
#include "logic_manager.h"

//body
#include "reg_body.h"
#include "login_body.h"

class Client;

class WorldSession
{
public:
    WorldSession();
    
    virtual ~WorldSession();
    
    //头处理
    void HandlePacket(Client* client);
private:
    void HandleWorld(int32 cmd, Client* client);
    //用户注册
    void UserRegistration(Client* client);
    //用户登录
    void UserLogin(Client* client);
};

#endif /* world_session_h */
