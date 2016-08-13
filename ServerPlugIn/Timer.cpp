
//
//  timemanager.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/12.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "Timer.h"


//class

Timer::Timer()
:rockid(0)
,delay_time(0)
,callId(0)
,callback(NULL){}

Timer::Timer(delay_t value, TASK_CALL func, int call_id)
:rockid(0)
,delay_time(value)
,callback(func)
,callId(call_id)
{
    
}

Timer::~Timer()
{
    Stop();
}

int Timer::rockId()const
{
    return rockid;
}

bool Timer::Start()
{
    return TimeManager::getInstance()->PushTimer(this);
}

void Timer::Stop()
{
    TimeManager::getInstance()->EndTimer(this);
}

bool Timer::isRunning()const
{
    return rockid > 0;
}

void Timer::setDelayTime(delay_t value)
{
    if(value < 0){
        delay_time = 0;
    }else{
        delay_time = value;
    }
}

void Timer::setReceipt(TASK_CALL func, int call_id)
{
    callback = func;
    callId = call_id;
}

//private
void Timer::rockId(int value)
{
    rockid = value;
}

struct timespec& Timer::happentime()
{
    return last;
}

void Timer::Reset(int value)
{
    trace("start ok:%d",value);
    rockId(value);
    int sec = (int)delay_time;                                  //间隔秒
    long msec = (delay_time - sec)*MVAL_TIME;                   //间隔毫秒
    //trace("end time = %d.%d",sec,msec);
    powder::ntime::gettime(&last);
    powder::ntime::addtime(&last, sec, (msec%MVAL_TIME)*UVAL_TIME);
}

