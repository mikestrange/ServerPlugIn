//
//  time_loop.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/9/5.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef time_loop_hpp
#define time_loop_hpp

#include "Timer.h"
#include "ticker.h"

class Timer;

class TimeLoop
{
    STATIC_CLASS(TimeLoop);
public:
    TimeLoop();
    virtual ~TimeLoop();
private:
    HashMap<uint32, Timer*> timeMap;
public:
    uint32 AddTime(Timer* target);
    
    bool DelTime(Timer* target);
    
public:
    SYS_TIME_T RunLoop();
};

#endif /* time_loop_hpp */
