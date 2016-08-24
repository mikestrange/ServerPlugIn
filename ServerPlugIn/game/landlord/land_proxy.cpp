//
//  land_proxy.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/17.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "land_proxy.h"


LandProxy::LandProxy()
:game(NULL)
{
    
}

LandProxy::LandProxy(LandlordLogic* target)
:game(target)
{
    
}

LandProxy::~LandProxy()
{
    
}


void LandProxy::onPacketProcess(PacketBuffer &packet)
{
    switch(packet.cmd)
    {
        case CMD_LAND_USER_ENTER:
            OnEnterLand(packet);
            break;
        case CMD_LAND_USER_EXIT:
            OnExitLand(packet);
            break;
        case CMD_LAND_SITDOWN:
            OnSitDown(packet);
            break;
        case CMD_LAND_STAND:
            OnStandUp(packet);
            break;
        case CMD_LAND_USER_ACTION:
            OnPlayerCardAction(packet);
            break;
        case CMD_LAND_HOLD_BANKER:
            OnHoldBankerAction(packet);
            break;
        default:
            trace("land no handle cmd:%d",packet.cmd);
        break;
    }
}

//游戏其他
void LandProxy::OnEnterLand(PacketBuffer& packet)
{
    USER_T user_id;
    packet>>user_id;
    game->EnterLand(packet.token_id, user_id);
}

void LandProxy::OnExitLand(PacketBuffer& packet)
{
    USER_T user_id;
    packet>>user_id;
    game->ExitLand(user_id);
}


void LandProxy::OnSitDown(PacketBuffer& packet)
{
    USER_T user_id;
    SEAT_T seat_id;
    packet>>user_id>>seat_id;
    game->SitDown(user_id, seat_id);
}

//游戏进行中，不允许退出
void LandProxy::OnStandUp(PacketBuffer& packet)
{
    USER_T user_id;
    packet>>user_id;
    game->StandUp(user_id);
}

void LandProxy::OnHoldBankerAction(PacketBuffer& packet)
{
    USER_T user_id;
    int8 mult;
    packet>>user_id>>mult;
    //抢庄
    auto player = game->allTab.find(user_id);
    if(player)
    {
        game->HoldBankerAction(player->getSeatId(), mult);
    }
}

void LandProxy::OnPlayerCardAction(PacketBuffer &packet)
{
    USER_T user_id;
    int8 atype;
    int8 count;
    packet>>user_id>>atype>>count;
    int8 nums[count];
    CARD_TYPE ctype;
    packet>>ctype;
    for(int8 i = 0; i < count; i++)
    {
        packet>>nums[i];
    }
    auto player = game->allTab.find(user_id);
    //出牌
    if(player)
    {
        game->PlayerCardAction(player->getUserId(), atype, nums, count, ctype);
    }
}



//send
void LandProxy::ToEnter(LandPlayer* player)
{
    PacketBuffer buf;
    buf.setHeader(CMD_LAND_USER_ENTER, HANDLE_CLIENT_MESSAGE, player->getTokenId());
    buf.WriteBegin();
    //写入要通知的人
    //写入用户数据
    buf.WriteEnd();
    
    GameLaunch::getInstance()->SendPacket(buf);
}

void LandProxy::ToExit(TOKEN_T tid, USER_T uid)
{
    PacketBuffer buf;
    buf.setHeader(CMD_LAND_USER_EXIT, HANDLE_CLIENT_MESSAGE, tid);
    buf.WriteBegin();
    //写入要通知的人
    buf<<uid;
    buf.WriteEnd();
    
    GameLaunch::getInstance()->SendPacket(buf);
}

void LandProxy::ToSitDown(TOKEN_T tid, SEAT_T sid)
{
    PacketBuffer buf;
    buf.setHeader(CMD_LAND_SITDOWN, HANDLE_CLIENT_MESSAGE, tid);
    buf.WriteBegin();
    //写入要通知的人
    buf<<sid;
    buf.WriteEnd();
    
    GameLaunch::getInstance()->SendPacket(buf);
}

void LandProxy::ToStandUp(TOKEN_T tid, SEAT_T sid)
{
    PacketBuffer buf;
    buf.setHeader(CMD_LAND_STAND, HANDLE_CLIENT_MESSAGE, tid);
    buf.WriteBegin();
    //写入要通知的人
    buf<<sid;
    buf.WriteEnd();
    
    GameLaunch::getInstance()->SendPacket(buf);
}






