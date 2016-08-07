//
//  timer.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/31.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "timer.h"

Timer::Timer()
:delay_time(0)
,is_time(false)
,delegate(NULL)
,type(0)
{

}

Timer::~Timer()
{
    stop();
}

bool Timer::start()
{
    is_time = TimeLoop::getInstance()->PushTime(this);
    if(!is_time) trace("计时器启动失败");
    return is_time;
}

void Timer::stop()
{
    is_time = false;
    TimeLoop::getInstance()->StopTime(this);
}

//delay_sec_t sec_t, long msec_t = 0
void Timer::setDelay(timer_t delay)
{
    if(delay < 0){
        delay_time = 0;
    }else{
        delay_time = delay;
    }
}

void Timer::setDelegate(TimeDelegate* delegate,int type)
{
    this->delegate = delegate;
    this->type = type;
}

bool Timer::isRunning()const
{
    return is_time;
}


//private
void Timer::Reset()
{
    __darwin_time_t sec = (__darwin_time_t)floor(delay_time);   //间隔秒
    long msec = (delay_time - sec)*MVAL_TIME;                   //间隔毫秒
    //trace("end time = %d.%d",sec,msec);
    powder::ntime::gettime(&last);
    powder::ntime::addtime(&last, sec, (msec%MVAL_TIME)*UVAL_TIME);
}

struct timespec& Timer::happentime()
{
    return last;
}

void Timer::UnStart()
{
    is_time = false;
}

void Timer::PushTask()
{
    if(delegate){
        delegate->perform(type);
    }else{
        trace("this time is no delegate");
    }
}


//static
bool Timer::TIME_COMPLETE(Timer* time)
{
    struct timespec now;
    powder::ntime::gettime(&now);
    struct timespec& last = time->happentime();
    //trace("%ld %ld %ld %ld",now.tv_sec, last.tv_sec,now.tv_nsec,last.tv_nsec);
    if(now.tv_sec > last.tv_sec) return true;
    if(now.tv_sec == last.tv_sec && now.tv_nsec >= last.tv_nsec) return true;
    return false;
}

bool Timer::TIME_EXCEED(Timer* timer, Timer* other)
{
    struct timespec& value1 = timer->happentime();
    struct timespec& value2 = other->happentime();
    if(value1.tv_sec > value2.tv_sec) return true;
    if(value1.tv_sec == value2.tv_sec && value1.tv_nsec >= value2.tv_nsec) return true;
    return false;
}


