//
//  world_session.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef world_session_h
#define world_session_h

#include <stdio.h>

#include "command.h"
#include "byte_buffer.h"
#include "client.h"

#include "string_utils.h"
#include "data_bank.h"
#include "logic_manager.h"

class Client;

class WorldSession : public Command
{
public:
    WorldSession()
    {
        Register();
    }
    
    virtual ~WorldSession()
    {
        Destroy();
    }
    void Register()override;
    void Destroy()override{};
    //头处理
    void HandlePacket(Client* client);
private:
    //用户注册
    void UserRegistration(PacketHeader& data, Client* client);
    //用户登录
    void UserLogin(PacketHeader& data, Client* client);
};

#endif /* world_session_h */
