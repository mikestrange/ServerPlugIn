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


void LandProxy::onPacketProcess(PacketBuffer &data)
{
    switch(data.cmd)
    {
        case CMD_LAND_USER_ENTER:
            EnterLand(data);
            break;
        case CMD_LAND_USER_EXIT:
            ExitLand(data);
            break;
        case CMD_LAND_SITDOWN:
            SitDown(data);
            break;
        case CMD_LAND_STAND:
            StandUp(data);
            break;
        case CMD_LAND_USER_ACTION:
            PlayerCardAction(data);
            break;
        case CMD_LAND_HOLD_BANKER:
            HoldBankerAction(data);
            break;
        default:
            trace("land no handle cmd:%d",data.cmd);
            break;
    }
}

//游戏其他
void LandProxy::SitDown(PacketBuffer& data)
{
    USER_T user_id;
    SEAT_T seat_id;
    data>>user_id>>seat_id;
    game->SitDown(user_id, seat_id);
}

//游戏进行中，不允许退出
void LandProxy::StandUp(PacketBuffer& data)
{
    USER_T user_id;
    data>>user_id;
    game->StandUp(user_id);
}

void LandProxy::EnterLand(PacketBuffer& data)
{
    USER_T user_id;
    data>>user_id;
    game->EnterLand(user_id);
}

void LandProxy::ExitLand(PacketBuffer& data)
{
    USER_T user_id;
    data>>user_id;
    game->ExitLand(user_id);
}

void LandProxy::HoldBankerAction(PacketBuffer& data)
{
    USER_T user_id;
    int8 mult;
    data>>user_id>>mult;
    //抢庄
    game->HoldBankerAction(user_id, mult);
}

void LandProxy::PlayerCardAction(PacketBuffer &data)
{
    USER_T user_id;
    int8 atype;
    int8 count;
    data>>user_id>>atype>>count;
    int8 nums[count];
    CARD_TYPE ctype;
    data>>ctype;
    for(int8 i = 0; i < count; i++)
    {
        data>>nums[i];
    }
    //出牌
    game->PlayerCardAction(user_id, atype, nums, count, ctype);
}






