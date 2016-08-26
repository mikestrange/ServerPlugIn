
//
//  view_proxy.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/19.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "game_proxy.h"


GameProxy::GameProxy()
{
    
}

GameProxy::~GameProxy()
{
    
}

void GameProxy::HandlePacket(PacketBuffer& packet)
{
    switch (packet.getCmd())
    {
        case SERVER_CMD_POTHOOK_REG:
                OnHookReg(packet);
            break;
        case SERVER_CMD_POTHOOK_UNREG:
                OnHookUnReg(packet);
            break;
        default:
            break;
    }
}


void GameProxy::OnHookReg(PacketBuffer& packet)
{
    int code;
    int32 roomid;
    uint8 type;
    packet>>code>>roomid>>type;
    if(code == 0)
    {
        if(type == POT_GAME_LAND_SERVER)
        {
            HomeManager::getInstance()->AddLogic(roomid, new LandlordLogic(roomid));
        }
    }
    Log::debug("返回我要注册一个房间 code = %d", code);
}

void GameProxy::OnHookUnReg(PacketBuffer& packet)
{
    int32 roomid;
    packet>>roomid;
    HomeManager::getInstance()->RemoveLogic(roomid);
    Log::debug("返回我要撤销一个房间 roomid = %d", roomid);
}

//send
void GameProxy::ActHookReg(int32 roomid, uint8 type)
{
    PacketBuffer buf;
    buf.setBegin(SERVER_CMD_POTHOOK_REG, HANDLE_WORLD_MESSAGE);
    buf.WriteBegin();
    buf<<roomid<<type;
    buf.WriteEnd();
    Log::debug("我要注册一个房间 %d",roomid);
    GameLaunch::getInstance()->SendPacket(buf);
}

void GameProxy::ActHookUnReg(int32 roomid)
{
    Log::debug("我要删除一个房间 %d",roomid);
    PacketBuffer buf;
    buf.setBegin(SERVER_CMD_POTHOOK_UNREG, HANDLE_WORLD_MESSAGE);
    buf.WriteBegin();
    buf<<roomid;
    buf.WriteEnd();
    GameLaunch::getInstance()->SendPacket(buf);
}