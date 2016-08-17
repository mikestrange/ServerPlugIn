//
//  user.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/14.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "user.h"

User::User()
:token_id(0)
,user_id(0)
,gate_id(0)
,status(0)
,appid(0)
,gametype(0){}

User::~User(){}

bool User::isLogin()const
{
    return user_id > 0;
}