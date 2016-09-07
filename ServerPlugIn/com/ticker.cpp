//
//  ticker.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/9/5.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "ticker.h"

//private
class Ticker
{
public:
    uint32 tick_id;
    SYS_TIME_T run_time;
    ITickHandler* target;
    int type;
    
    Ticker(uint32 tick_id, SYS_TIME_T delay_time, ITickHandler* target, int type = 0)
    :tick_id(tick_id)
    ,target(target)
    ,type(type)
    {
        run_time = powder::getTimer() + delay_time;
    };
    
    void Futile()
    {
        target = NULL;
    }
    
    bool isFutile()
    {
        return target == NULL;
    }
    
    void OnTicker()
    {
        if(target)
        {
            target->OnTimeProcess(type);
        }
    }
    
    virtual ~Ticker(){};
};

//static
static HashMap<uint32, Ticker*> tickMap;
//static Locked m_lock;
static uint32 currentId = 0;

uint32 CreateTicker(ITickHandler* target, SYS_TIME_T delay_time, int type)
{
    //AUTO_LOCK(&m_lock);
    if(tickMap.has(++currentId))
    {
        if(currentId == UINT32_MAX) currentId = 0;
    }
    uint32 tick_id = currentId;
    tickMap.put(tick_id, new Ticker(tick_id, delay_time, target, type));
    return tick_id;
}

//返回最近的一个时间
SYS_TIME_T CompleteTickers(std::vector<uint32>& ticks)
{
    //AUTO_LOCK(&m_lock);
    HashMap<uint32, Ticker*>::Iterator iter;
    SYS_TIME_T sys_time = 0;
    SYS_TIME_T current_time = powder::getTimer();
    //遍历获取
    for(iter = tickMap.begin(); iter!=tickMap.end(); iter++)
    {
        auto tick = iter->second;
        if(current_time >= tick->run_time || tick->isFutile())
        {
            ticks.push_back(tick->tick_id);
        }else{
            if(sys_time == 0){
                sys_time = tick->run_time;
            }else{
                if(sys_time > tick->run_time)
                {
                    sys_time = tick->run_time;
                }
            }
        }
    }
    return sys_time;
}


void ApplyTicker(uint32 tid)
{
    auto tick = tickMap.remove(tid);
    if(tick)
    {
        tick->OnTicker();
    }
    SAFE_DELETE(tick);
}

void StopTicker(uint32 tid)
{
    auto tick = tickMap.find(tid);
    if(tick)
    {
        tick->Futile();
    }
}
