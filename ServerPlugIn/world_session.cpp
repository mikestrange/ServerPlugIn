//
//  world_session.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "world_session.h"

#include "reg_body.h"
#include "login_body.h"

static DataBank bank;

static void HandleLogic(PacketHeader& data, Client* client)
{
    ByteBuffer buffer;
    buffer.WriteBegin();
    buffer.WriteObject(data);
    //uid(追加字段)
    uint32 uid = 10086;
    buffer<<uid;
    //剩余子节写入
    client->packet().ReadBuffer(buffer, client->subLeng());
    buffer.WriteEnd();
    //房间处理
    LogicManager::getInstance()->HandlePacket(buffer);
}

//
void WorldSession::Register()
{
    SetCommand(SERVER_CMD_USER_REG, handler_2(this, &WorldSession::UserRegistration));
    SetCommand(SERVER_CMD_USER_LOGIN, handler_2(this, &WorldSession::UserLogin));
}

//
void WorldSession::HandlePacket(Client *client)
{
    PacketHeader base(client->packet());
    switch(base.msgType)
    {
        case HANDLE_WORLD_MESSAGE:
                //世界处理
                SendToCommand(base.cmd, base, client);
            break;
        case HANDLE_GAME_MESSAGE:
                //游戏处理
                HandleLogic(base, client);
            break;
        case HANDLE_HALL_MESSAGE:
            
            break;
        default:
            trace("no handle message: %d", base.msgType);
            break;
    }
}

void WorldSession::UserRegistration(PacketHeader& data, Client* client)
{
    trace(">>UserRegistration");
    RegBody info(client->packet());
    
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

void WorldSession::UserLogin(PacketHeader& data, Client* client)
{
    trace(">>do UserLogin");
    LoginBody info(client->packet());
    
    if(!StringUtil::scope(info.password, 6, 24))
    {
        trace("login error: password length is error(6, 24)");
        client->Disconnect();
        return;
    }
    
    if(!StringUtil::scope(info.macbind, 6, 128))
    {
        trace("login error: macbind length is error(6, 128)");
        client->Disconnect();
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
        client->Disconnect();
    }
}