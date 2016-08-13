//
//  timemanager.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/12.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef rock_h
#define rock_h

#include <map>
#include <vector>

#include "global.h"
#include "time_utils.h"
#include "TimeLoop.h"
#include "task.h"

class TimeManager;

//计时器时间
typedef double delay_t;

class Timer
{
private:
    int rockid;
    delay_t delay_time;
private:
    struct timespec last;
    
public:
    Timer();
    Timer(delay_t value, TASK_CALL func, int call_id = 0);
    virtual ~Timer();
    
public:
    virtual bool Start();
    virtual void Stop();
    virtual int rockId()const;
    virtual bool isRunning()const;
    virtual void setDelayTime(delay_t value);
    virtual void setReceipt(TASK_CALL func, int call_id = 0);
    
private:
    friend class RockManager;
    int callId;
    friend class RockManager;
    TASK_CALL callback;
    
    friend class TimeManager;
    void Reset(int value);
    
    friend class TimeManager;
    void rockId(int value);
    
    friend class TimeManager;
    struct timespec& happentime();
};




#endif /* timemanager_h */
