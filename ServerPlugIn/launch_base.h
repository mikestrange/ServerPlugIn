//
//  logic_base.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/13.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef logic_base_h
#define logic_base_h

#include "global.h"
#include "packet_buffer.h"
#include "client.h"

//代理
class IProxy
{
    
};

//会话
class ISession : public IProxy
{
public:
    virtual void OnPacketHandle(int fd, PacketBuffer* packet)=0;
};


class LaunchBase
{
public:
    LaunchBase(){};
    virtual ~LaunchBase(){};
    
public:
    virtual void Launch()=0;
    virtual void UnLaunch()=0;
};

#endif /* logic_base_h */
