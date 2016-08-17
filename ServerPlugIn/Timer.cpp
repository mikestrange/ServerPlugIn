
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
:time_id(0)
,delay_time(0)
,delegate(NULL)
,call_type(0){}

Timer::Timer(delay_t value, TimeOutEvent* target, int ctype)
:time_id(0)
,delay_time(value)
,delegate(target)
,call_type(ctype)
{
    
}

Timer::~Timer()
{
    Stop();
}

int Timer::rockId()const
{
    return time_id;
}

bool Timer::Start()
{
    Stop();
    return TimeManager::getInstance()->PushTimer(this);
}

void Timer::Stop()
{
    TimeManager::getInstance()->EndTimer(this);
}

bool Timer::isRunning()const
{
    return time_id > 0;
}

void Timer::setDelayTime(delay_t value)
{
    if(value < 0){
        delay_time = 0;
    }else{
        delay_time = value;
    }
}

void Timer::setDelegate(TimeOutEvent* target, int ctype)
{
    delegate = target;
    call_type = ctype;
}

//private
void Timer::rockId(int value)
{
    time_id = value;
}

struct timespec& Timer::happentime()
{
    return last;
}

void Timer::Reset(int value)
{
    trace("time start ok:%d", value);
    rockId(value);
    int sec = (int)delay_time;                                  //间隔秒
    long msec = (delay_time - sec)*MVAL_TIME;                   //间隔毫秒
    //trace("end time = %d.%d",sec,msec);
    powder::ntime::gettime(&last);
    powder::ntime::addtime(&last, sec, (msec%MVAL_TIME)*UVAL_TIME);
}

