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
#include "byte_buffer.h"

class LogicBase
{
public:
    LogicBase();
    virtual ~LogicBase();
    
public:
    //启动
    virtual void launch()=0;
    //废除
    virtual void revoke()=0;
    //
    virtual void process(ReadBytes& bytes)=0;
};

#endif /* logic_base_h */
