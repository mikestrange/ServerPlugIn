//
//  world_session.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "world_session.h"

static DataBank bank;

STATIC_CLASS_INIT(WorldSession);

//发送房间
static void HandleToGame(PacketBuffer& packet)
{
    //未登录的用户不允许通知房间
    if(!PlayerManager::getInstance()->HasPlayer(packet.getTokenId()))
    {
        Log::info("handle error:not login");
        return;
    }
    //这里作为发送的方式给游戏服务器
    PacketBuffer buf;
    buf.setBegin(packet.getCmd(), 0, packet.getTokenId(), packet.getViewId());
    buf.WriteBegin();
    //剩余子节写入
    packet.ReadBuffer(buf, packet.subLeng());
    buf.WriteEnd();
    //交给房间处理
    PotHook::getInstance()->SendHook(packet.getViewId(), buf);
}


void WorldSession::HandlePacket(SocketHandler& packet)
{
    Log::info("server message: cmd = %d, type = %d", packet.getCmd(), packet.getType());
    //类型处理
    switch(packet.getType())
    {
        case HANDLE_WORLD_MESSAGE:
                //世界处理
                HandleToWorld(packet);
            break;
        case HANDLE_GAME_MESSAGE:
                //游戏处理
                HandleToGame(packet);
            break;
        case HANDLE_HALL_MESSAGE:
            
            break;
        default:
            trace("no handle message: %d", base.msgType);
            break;
    }
}

//处理世界任务
void WorldSession::HandleToWorld(SocketHandler& packet)
{
    switch(packet.getCmd())
    {
        case SERVER_CMD_USER_REG:
            OnUserRegistration(packet);
        break;
        case SERVER_CMD_USER_LOGIN:
            OnUserLogin(packet);
            break;
        case SERVER_CMD_POTHOOK_REG:
            OnHookRegister(packet);
            break;
        case SERVER_CMD_POTHOOK_UNREG:
            OnHookUnRegister(packet);
            break;
        default:
            OnHookRegister(packet);
            break;
    }
}

void WorldSession::OnUserRegistration(SocketHandler& packet)
{
    trace(">>UserRegistration");
    RegBody info(packet);
    
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

void WorldSession::OnUserLogin(SocketHandler& packet)
{
    LoginBody info(packet);
    //--
    if(PlayerManager::getInstance()->HasUID(info.uid))
    {
        Log::info(">>do not reg aglin");
        return;
    };
    trace(">>do UserLogin");
    if(!StringUtil::scope(info.password, 6, 24))
    {
        Log::warn("login error: password length is error(6, 24)");
        packet.Disconnect();
        return;
    }
    
    if(!StringUtil::scope(info.macbind, 6, 128))
    {
        Log::warn("login error: macbind length is error(6, 128)");
        packet.Disconnect();
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
        player->sockfd = packet.GetSocketFd();
        PlayerManager::getInstance()->AddPlayer(player);
        Log::info("login OK: uid = %d", info.uid);
    }else{
        Log::warn("login error: is no this uid = %d", info.uid);
        packet.Disconnect();
    }
}

void WorldSession::OnHookRegister(SocketHandler& packet)
{
    uint32 regid;
    int8 rtype;
    //std::string key;//必须孝正合法性
    packet>>regid>>rtype;
    int code = PotHook::getInstance()->AddNode(packet.GetSocketFd(), regid, rtype);
    //通知游戏服务器
    PacketBuffer buf;
    buf.setBegin(SERVER_CMD_POTHOOK_REG, 0, 0, 0);
    buf.WriteBegin();
    buf<<code<<regid<<rtype;
    buf.WriteEnd();
    
    packet.SendPacket(buf);
}

void WorldSession::OnHookUnRegister(SocketHandler& packet)
{
    uint32 potid;
    packet>>potid;
    PotHook::getInstance()->DelByPotId(potid);
    //通知游戏服务器
    PacketBuffer buf;
    buf.setBegin(SERVER_CMD_POTHOOK_UNREG, 0, 0, 0);
    buf.WriteBegin();
    buf<<potid;
    buf.WriteEnd();
    
    packet.SendPacket(buf);
}
