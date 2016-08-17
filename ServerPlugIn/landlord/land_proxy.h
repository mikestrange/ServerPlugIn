//
//  land_proxy.hpp
//  ServerPlugIn
//  代理斗地主的网络事务(不做逻辑处理)
//  Created by MikeRiy on 16/8/17.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef land_proxy_h
#define land_proxy_h

#include "../landlord_logic.h"
#include "../packet_buffer.h"

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
    virtual void onPacketProcess(PacketBuffer &data);
private:
    void SitDown(PacketBuffer& data);
    void StandUp(PacketBuffer& data);
    void EnterLand(PacketBuffer& data);
    void ExitLand(PacketBuffer& data);
    
    void PlayerCardAction(PacketBuffer& data);
    void HoldBankerAction(PacketBuffer& data);
};

#endif /* land_proxy_h */
