//
//  timemanager.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/12.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef rock_h
#define rock_h

#include "global.h"
#include "time_utils.h"
#include "TimeLoop.h"
#include "task.h"

class TimeManager;

//计时回调
class TimeOutEvent
{
public:
    virtual void onTimeoutProcess(int type)=0;
};

//计时器时间
typedef double delay_t;

class Timer
{
private:
    int time_id;
    delay_t delay_time;
private:
    struct timespec last;
    
public:
    Timer();
    Timer(delay_t value, TimeOutEvent* target, int ctype = 0);
    virtual ~Timer();
    
public:
    virtual bool Start();
    virtual void Stop();
    virtual int rockId()const;
    virtual bool isRunning()const;
    virtual void setDelayTime(delay_t value);
    virtual void setDelegate(TimeOutEvent *target, int ctype = 0);
    
private:
    friend class RockManager;
    int call_type;
    
    friend class RockManager;
    TimeOutEvent* delegate;
    
    friend class TimeManager;
    void Reset(int value);
    
    friend class TimeManager;
    void rockId(int value);
    
    friend class TimeManager;
    struct timespec& happentime();
};




#endif /* timemanager_h */
