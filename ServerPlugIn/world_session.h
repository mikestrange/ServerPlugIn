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

class Client;

class WorldSession : public Command
{
public:
    WorldSession()
    {
        Register();
    }
    void Register()override;
    void Destroy()override{};
private:
    //用户注册
    void UserRegistration(Client* client);
    //用户登录
    void UserLogin(Client* client);
};

#endif /* world_session_h */
