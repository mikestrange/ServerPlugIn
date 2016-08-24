//
//  land_proxy.hpp
//  ServerPlugIn
//  代理斗地主的网络事务(不做逻辑处理)
//  Created by MikeRiy on 16/8/17.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef land_proxy_h
#define land_proxy_h

#include "landlord_logic.h"

#include "../GameLaunch.h"
//#include "../../packet_buffer.h"


class LandlordLogic;
class PacketBuffer;

class LandProxy
{
private:
    LandlordLogic* game;
public:
    LandProxy();
    LandProxy(LandlordLogic* target);
    virtual ~LandProxy();
public:
    virtual void onPacketProcess(PacketBuffer &packet);
private:
    //recv
    void OnEnterLand(PacketBuffer& packet);
    void OnExitLand(PacketBuffer& packet);
    void OnSitDown(PacketBuffer& packet);
    void OnStandUp(PacketBuffer& packet);
    
    void OnPlayerCardAction(PacketBuffer& packet);
    void OnHoldBankerAction(PacketBuffer& packet);
    
    
public:
    //send
    void ToEnter(LandPlayer* player);
    void ToExit(TOKEN_T tid, USER_T uid);
    void ToSitDown(TOKEN_T tid, SEAT_T sid);
    void ToStandUp(TOKEN_T tid, SEAT_T sid);
};

#endif /* land_proxy_h */
