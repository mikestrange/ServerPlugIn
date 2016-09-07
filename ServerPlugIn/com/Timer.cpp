
//
//  timemanager.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/12.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "Timer.h"

Timer::Timer()
:time_id(0)
,m_event(NULL)
,m_type(0){}

Timer::Timer(SYS_TIME_T value)
:delay_time(value)
,m_event(NULL)
,m_type(0)
{
    
}

Timer::Timer(SYS_TIME_T value, ITickHandler* event, int type)
{
    setDelay(value);
    setEventHandler(event, type);
}

Timer::~Timer()
{
    Stop();
}

void Timer::setDelay(SYS_TIME_T value)
{
    delay_time = value;
}

void Timer::setEventHandler(ITickHandler* event, int type)
{
    m_event = event;
    m_type = type;
}

void Timer::Start()
{
    TimeLoop::getInstance()->AddTime(this);
    //通知
    if(isRunning())
    {
        powder::ResumeMain();
    };
}

void Timer::Stop()
{
    if(TimeLoop::getInstance()->DelTime(this))
    {
        powder::ResumeMain();
    };
}

bool Timer::isRunning()const
{
    return time_id > 0;
}

void Timer::UnStart()
{
    time_id = 0;
}

