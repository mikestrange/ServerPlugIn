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

void WorldSession::OnPacketHandle(SOCKET_T sockfd, SocketHandler& packet)
{
    Log::info("world message: cmd = %d, type = %d", packet.getCmd(), packet.getType());
    //类型处理
    switch(packet.getType())
    {
        case HANDLE_WORLD_MESSAGE:
                //世界处理
                HandleToWorld(packet);
            break;
        case HANDLE_GAME_MESSAGE:
                //游戏处理(挂钩)
                HandleToHook(packet);
            break;
        case HANDLE_HALL_MESSAGE:
                //大厅也是挂钩
                HandleToHook(packet);
            break;
        case HANDLE_CLIENT_MESSAGE:
                //通知世界玩家
                HandleToPlayer(packet);
            break;
        default:
                Log::debug("world no handle message: %d", packet.getType());
            break;
    }
}

//世界处理
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

//通知挂钩处理(未登录的玩家不允许访问)
void WorldSession::HandleToHook(SocketHandler& packet)
{
    //未登录的用户不允许通知房间
    if(!PlayerManager::getInstance()->HasPlayer(packet.getTokenId()))
    {
        Log::info("handle error:not login");
        return;
    }
    //可以完全不用拷贝
    PacketBuffer buf;
    buf.CopyBegin(packet);
    buf.WriteBegin();
    packet.ReadBuffer(buf, packet.subLeng());
    buf.WriteEnd();
    //发送到挂钩
    HookManager::getInstance()->SendHook(packet.getViewId(), buf);
}

//通知玩家所在的连接(通过tokenid)
void WorldSession::HandleToPlayer(SocketHandler& packet)
{
    auto player = PlayerManager::getInstance()->GetPlayer(packet.getTokenId());
    if(player)
    {
        auto m_sock = WorldServer::getInstance()->getSocketHandler(player->sockfd);
        if(m_sock)
        {
            //可以完全不用拷贝
            PacketBuffer buf;
            buf.CopyBegin(packet);
            buf.WriteBegin();
            packet.ReadBuffer(buf, packet.subLeng());
            buf.WriteEnd();
            //对应的连接器
            m_sock->SendPacket(buf);
        }
    }
}

void WorldSession::OnUserRegistration(SocketHandler& packet)
{
    RegBody info(packet);
    int code = 0;
    if(!StringUtil::scope(info.name, 1, 24))
    {
        Log::warn("reg error: name length is error(1, 24)");
        code = 1;
    }else if(!StringUtil::scope(info.password, 6, 24)){
        Log::warn("reg error: password length is error(6, 24)");
        code = 2;
    }else if(!StringUtil::scope(info.macbind, 6, 128)){
        Log::warn("reg error: macbind length is error(6, 128)");
        code = 3;
    }else{
        bank.test_connent();
        DataQuery query;
        //注册
        if(info.uid == 0){
            //绑定存在
            bank.findFormat(query, "select * from user_info.account where macbind ='%s'", info.macbind.c_str());
        }else{
            //uid存在
            bank.findFormat(query, "select * from user_info.account where uid ='%d'", info.uid);
        }
        //uid = 0为递归注册id
        if(query.empty()){
            Log::info("reg OK");
            bank.applyFormat("INSERT INTO user_info.account (uid, appid, name, password, macbind) VALUES ('%d','%d','%s','%s','%s')", info.uid, info.appid, info.name.c_str(),
                             info.password.c_str(), info.macbind.c_str());
        }else{
            Log::warn("reg error: is have this uid = %d macbind = %s", info.uid, info.macbind.c_str());
            code = 4;
        }
    }
    //返回
    if(code == 0){
        
    }else{
        
        packet.Disconnect();
    }
}

void WorldSession::OnUserLogin(SocketHandler& packet)
{
    LoginBody info(packet);
    //--
    int32 code = 0;
    if(!StringUtil::scope(info.password, 6, 24))
    {
        Log::warn("login error: password length is error(6, 24)");
        code = 2;
    }else if(!StringUtil::scope(info.macbind, 6, 128))
    {
        Log::warn("login error: macbind length is error(6, 128)");
        code = 3;
    }else{
        bank.test_connent();
        DataQuery query;
        //uid是否存在
        bank.findFormat(query, "select * from user_info.account where uid ='%d' and macbind='%s' and password='%s'",
                        info.uid, info.macbind.c_str(), info.password.c_str());
        //未注册的用户
        if(query.empty())
        {
            Log::warn("login error: is no this uid = %d", info.uid);
            code = 4;
        }
    }
    //返回
    if(code == 0){
        //踢掉之前的登录用户
        auto player = new Player;
        player->token_id = info.uid;
        player->user_id = info.uid;
        player->sockfd = packet.GetSocketFd();
        PlayerManager::getInstance()->AddPlayer(player);
    }else{
        
        packet.Disconnect();
    }
}

void WorldSession::OnHookRegister(SocketHandler& packet)
{
    uint32 potid;
    int8 type;
    //std::string key;//必须孝正合法性
    packet>>potid>>type;
    int code = HookManager::getInstance()->AddNode(packet.GetSocketFd(), potid, type);
    //通知游戏服务器
    PacketBuffer buf;
    buf.setBegin(SERVER_CMD_POTHOOK_REG, 0, 0, 0);
    buf.WriteBegin();
    buf<<code<<potid<<type;
    buf.WriteEnd();
    //直接返回
    packet.SendPacket(buf);
}

void WorldSession::OnHookUnRegister(SocketHandler& packet)
{
    uint32 potid;
    packet>>potid;
    HookManager::getInstance()->DelByPotId(potid);
    //通知游戏服务器
    PacketBuffer buf;
    buf.setBegin(SERVER_CMD_POTHOOK_UNREG, 0, 0, 0);
    buf.WriteBegin();
    buf<<potid;
    buf.WriteEnd();
    //直接返回
    packet.SendPacket(buf);
}
