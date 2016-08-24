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
static void HandleToGame(Client* client)
{
    PacketHeader& data = client->packet;
    //未登录的用户不允许通知房间
    if(!PlayerManager::getInstance()->HasPlayer(data.token_id))
    {
        Log::info("handle error:not login");
        return;
    }
    //这里作为发送的方式给游戏服务器
    PacketBuffer buf;
    buf.setHeader(data.cmd, 0, data.token_id, data.view_id);
    buf.WriteBegin();
    //剩余子节写入
    client->packet.ReadBuffer(buf, client->packet.subLeng());
    buf.WriteEnd();
    //交给房间处理
    PotHook::getInstance()->SendHook(data.view_id, &buf[0], buf.wsize());
}


void WorldSession::HandlePacket(Client* client)
{
    PacketHeader& data = client->packet;
    //Log::info("server message[%d]: cmd = %d, type = %d, view = %d, version = %d", client->user.user_id, base.cmd, base.msgType, base.viewid, base.version);
    //类型处理
    switch(data.type)
    {
        case HANDLE_WORLD_MESSAGE:
                //世界处理
                HandleToWorld(client);
            break;
        case HANDLE_GAME_MESSAGE:
                //游戏处理
                HandleToGame(client);
            break;
        case HANDLE_HALL_MESSAGE:
            
            break;
        default:
            trace("no handle message: %d", base.msgType);
            break;
    }
}

//处理世界任务
void WorldSession::HandleToWorld(Client* client)
{
    PacketHeader& data = client->packet;
    switch(data.cmd)
    {
        case SERVER_CMD_USER_REG:
            OnUserRegistration(client);
        break;
        case SERVER_CMD_USER_LOGIN:
            OnUserLogin(client);
            break;
        case SERVER_CMD_POTHOOK_REG:
            OnHookRegister(client);
            break;
        case SERVER_CMD_POTHOOK_UNREG:
            OnHookUnRegister(client);
            break;
        default:
            break;
    }
}

void WorldSession::OnUserRegistration(Client* node)
{
    trace(">>UserRegistration");
    RegBody info(node->packet);
    
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

void WorldSession::OnUserLogin(Client* client)
{
    LoginBody info(client->packet);
    //--
    if(PlayerManager::getInstance()->HasPlayerByUID(info.uid))
    {
        Log::info(">>do not reg aglin");
        return;
    };
    trace(">>do UserLogin");
    if(!StringUtil::scope(info.password, 6, 24))
    {
        Log::warn("login error: password length is error(6, 24)");
        client->getSock()->setClose();
        return;
    }
    
    if(!StringUtil::scope(info.macbind, 6, 128))
    {
        Log::warn("login error: macbind length is error(6, 128)");
        client->getSock()->setClose();
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
        auto player = new Player;
        player->token_id = info.uid;
        player->user_id = info.uid;
        player->fd = client->getSock()->getFd();
        PlayerManager::getInstance()->AddPlayer(player);
        Log::info("login OK: uid = %d", info.uid);
    }else{
        Log::warn("login error: is no this uid = %d", info.uid);
        client->getSock()->setClose();
    }
}

void WorldSession::OnHookRegister(Client *client)
{
    uint32 regid;
    int8 rtype;
    //std::string key;//必须孝正合法性
    client->packet>>regid>>rtype;
    int code = PotHook::getInstance()->AddNode(client->getSock()->getFd(), regid, rtype);
    //通知游戏服务器
    PacketBuffer buf;
    buf.setHeader(SERVER_CMD_POTHOOK_REG, 0, 0, 0);
    buf.WriteBegin();
    buf<<code<<regid<<rtype;
    buf.WriteEnd();
    client->getSock()->Send(&buf[0], buf.wsize());
}

void WorldSession::OnHookUnRegister(Client *client)
{
    uint32 regid;
    client->packet>>regid;
    PotHook::getInstance()->DelByNodeId(regid);
    //通知游戏服务器
    PacketBuffer buf;
    buf.setHeader(SERVER_CMD_POTHOOK_UNREG, 0, 0, 0);
    buf.WriteBegin();
    buf<<regid;
    buf.WriteEnd();
    client->getSock()->Send(&buf[0], buf.wsize());
}
