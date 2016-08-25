//
//  game_proxy.h
//  ServerPlugIn
//  协议处理
//  Created by MikeRiy on 16/8/19.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef game_proxy_h
#define game_proxy_h

#include "packet_buffer.h"
#include "command.h"
#include "GameLaunch.h"
#include "home_manager.h"

class GameProxy
{
    
public:
    GameProxy();
    
    virtual ~GameProxy();
    
public:
    void HandlePacket(PacketBuffer& packet);
    
private:
    void OnHookReg(PacketBuffer& packet);
    void OnHookUnReg(PacketBuffer& packet);
    
    //send
public:
    void ToHookReg(int32 roomid, uint8 type);
    void ToHookUnReg(int32 roomid);
};

#endif /* game_proxy_h */
