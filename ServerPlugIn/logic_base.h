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

//代理
class LogicProxy
{

};

class LogicBase
{
public:
    LogicBase(){};
    virtual ~LogicBase(){};
    
public:
    virtual void Launch()=0;
    virtual void UnLaunch()=0;
public:
    virtual void onProcess(PacketBuffer& data)=0;
};

#endif /* logic_base_h */
