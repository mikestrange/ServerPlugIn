//
//  game_base.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/23.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef game_base_hpp
#define game_base_hpp

#include "packet_buffer.h"

class GameBase
{
public:
    GameBase();
    virtual ~GameBase();
    virtual void HandlePacket(PacketBuffer& packet)=0;
    virtual void Launch()=0;
    virtual void UnLaunch()=0;
};
#endif /* game_base_hpp */
