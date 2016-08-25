//
//  gate_proxy.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/19.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef gate_proxy_hpp
#define gate_proxy_hpp

#include "global.h"
#include "packet_buffer.h"

class GateProxy
{
public:
    GateProxy();
    virtual ~GateProxy();
public:
    virtual void HandlePacket(PacketBuffer& packet);
};

#endif /* gate_proxy_hpp */
