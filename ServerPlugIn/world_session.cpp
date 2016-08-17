//
//  world_session.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "world_session.h"

static DataBank bank;

WorldSession::WorldSession()
{
    
}

WorldSession::~WorldSession()
{
    
}

//发送房间
static void HandleLogic(Client* client)
{
    //未登录
    if(!client->user.isLogin())
    {
        Log::info("handle error:not login");
        return;
    }
    //这里作为发送的方式给游戏服务器
    PacketBuffer buffer;
    buffer.CopyHeader(client->packet);
    buffer.WriteBegin();
    //uid(追加字段)
    buffer<<client->user.user_id;
    //剩余子节写入
    client->packet.ReadBuffer(buffer, client->packet.subLeng());
    buffer.WriteEnd();
    //交给房间处理
    LogicManager::getInstance()->HandlePacket(buffer);
}

//
void WorldSession::HandlePacket(Client *client)
{
    PacketHeader& base = client->packet;
    //
    switch(base.msgType)
    {
        case HANDLE_WORLD_MESSAGE:
                //世界处理
                HandleWorld(base.cmd, client);
            break;
        case HANDLE_GAME_MESSAGE:
                //游戏处理
                HandleLogic(client);
            break;
        case HANDLE_HALL_MESSAGE:
            
            break;
        default:
            trace("no handle message: %d", base.msgType);
            break;
    }
}

//处理世界任务
void WorldSession::HandleWorld(int32 cmd, Client* client)
{
    PacketHeader& base = client->packet;
    switch(base.cmd)
    {
        case SERVER_CMD_USER_REG:
            UserRegistration(client);
        break;
        case SERVER_CMD_USER_LOGIN:
            UserLogin(client);
        break;
    }
}

void WorldSession::UserRegistration(Client* client)
{
    trace(">>UserRegistration");
    RegBody info(client->packet);
    
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
    if(client->user.isLogin())
    {
        Log::info(">>do not reg aglin");
        return;
    };
    trace(">>do UserLogin");
    LoginBody info(client->packet);
    
    if(!StringUtil::scope(info.password, 6, 24))
    {
        Log::info("login error: password length is error(6, 24)");
        client->Disconnect();
        return;
    }
    
    if(!StringUtil::scope(info.macbind, 6, 128))
    {
        Log::info("login error: macbind length is error(6, 128)");
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
        client->user.user_id = info.uid;
        Log::info("login OK: uid = %d", info.uid);
    }else{
        Log::info("login error: is no this uid = %d", info.uid);
        client->Disconnect();
    }
}