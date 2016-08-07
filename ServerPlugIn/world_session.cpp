//
//  world_session.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "world_session.h"

#include "reginfo.h"
#include "logininfo.h"

static DataBank bank;


void WorldSession::Register()
{
    SetCommand(SERVER_USER_REG, handler_1(this, &WorldSession::UserRegistration));
    SetCommand(SERVER_USER_LOGIN, handler_1(this, &WorldSession::UserLogin));
}

void WorldSession::UserRegistration(Client* client)
{
    trace("注册啊");
    RegInfo info(client->packet());
    
    if(!StringUtil::scope(info.name, 1, 24))
    {
        trace("reg error: name length is error(1, 24)");
        return;
    }
    
    if(!StringUtil::scope(info.password, 6, 24))
    {
        trace("reg error: password length is error(6, 24)");
        return;
    }
    
    if(!StringUtil::scope(info.macbind, 6, 128))
    {
        trace("reg error: macbind length is error(6, 128)");
        return;
    }
    
    bank.test_connent();
    DataQuery query;
    
    if(info.uid == 0){
        //绑定存在
        bank.findFormat(query, "select * from user_info.account where macbind ='%s'", info.macbind.c_str());
    }else{
        //uid存在
        bank.findFormat(query, "select * from user_info.account where uid ='%d'", info.uid);
    }
    //
    if(query.length() > 0){
        trace("reg error: is have this uid = %d macbind = %s", info.uid, info.macbind.c_str());
    }else{
        trace("reg OK");
        bank.applyFormat("INSERT INTO user_info.account (uid, appid, name, password, macbind) VALUES ('%d','%d','%s','%s','%s')", info.uid, info.appid, info.name.c_str(),
                         info.password.c_str(), info.macbind.c_str());
    }
}

void WorldSession::UserLogin(Client* client)
{
    trace("登陆啊");
    LoginInfo info(client->packet());
    
    if(!StringUtil::scope(info.password, 6, 24))
    {
        trace("login error: password length is error(6, 24)");
        return;
    }
    
    if(!StringUtil::scope(info.macbind, 6, 128))
    {
        trace("login error: macbind length is error(6, 128)");
        return;
    }
    
    bank.test_connent();
    DataQuery query;
    //uid是否存在
    bank.findFormat(query, "select * from user_info.account where uid ='%d' and macbind='%s' and password='%s'",
                    info.uid, info.macbind.c_str(), info.password.c_str());
    //
    if(query.length() > 0)
    {
        trace("login OK: uid = %d", info.uid);
    }else{
        trace("login error: is no this uid = %d", info.uid);
    }
}