//
//  ticker.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/9/5.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef ticker_hpp
#define ticker_hpp

#include "lock.h"
#include "hash_map.h"
#include "time_utils.h"
#include "global.h"

#include <vector>


class ITickHandler
{
public:
    virtual void OnTimeProcess(int type)=0;
    
};

uint32 CreateTicker(ITickHandler* target, SYS_TIME_T delay_time = 0, int type = 0);

SYS_TIME_T CompleteTickers(std::vector<uint32>& ticks);

void ApplyTicker(uint32 tid);

void StopTicker(uint32 tid);

#endif /* ticker_hpp */
