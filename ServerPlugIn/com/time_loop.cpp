//
//  time_loop.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/9/5.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "time_loop.h"

STATIC_CLASS_INIT(TimeLoop);


TimeLoop::TimeLoop()
{
    
}

TimeLoop::~TimeLoop()
{
    
}

uint32 TimeLoop::AddTime(Timer* target)
{
    DelTime(target);
    //建立一个
    uint32 tick_id = CreateTicker(target->m_event, target->delay_time, target->m_type);
    target->time_id = tick_id;
    //添加计时器
    timeMap.put(tick_id, target);
    return tick_id;
}

bool TimeLoop::DelTime(Timer* target)
{
    if(target && target->isRunning())
    {
        //停止(不删除,下次响应计时器删除)
        StopTicker(target->time_id);
        //删除一个计时器
        timeMap.remove(target->time_id);
        target->UnStart();
        return true;
    }
    return false;
}

SYS_TIME_T TimeLoop::RunLoop()
{
    std::vector<uint32> ticks;
    SYS_TIME_T sys_time = CompleteTickers(ticks);
    if(!ticks.empty())
    {
        std::vector<uint32>::iterator iter;
        for(iter = ticks.begin(); iter!= ticks.end(); ++iter)
        {
            uint32 tick_id = *iter;
            //改变自己状态
            auto s_timer = timeMap.remove(tick_id);
            if(s_timer)
            {
                s_timer->UnStart();
            }
            ApplyTicker(tick_id);
        }
    }
    return sys_time;
}