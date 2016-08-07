//
//  time_loop.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/31.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "time_loop.h"

TimeLoop* TimeLoop::_instance = NULL;

static void push_thread(Timer* time)
{
    //将执行的异步匿名函数
    static auto func = block(int type, void* args)
    {
        Timer* time = (Timer*)args;
        time->PushTask();
    };
    //异步推送
    powder::AsynPush(Task::create(func, time));
}

TimeLoop* TimeLoop::getInstance()
{
    if(NULL == _instance)
    {
        _instance = new TimeLoop();
        //_instance->name = "time line:";
        _instance->start();
    }
    return _instance;
}

void TimeLoop::run()
{
    while(isRunning())
    {
        std::list<Timer*> handles;
        Timer* near = complete_times(handles);
        //真空状态,不受线程保护(暂时试试)
        std::list<Timer*>::iterator iter;
        for(iter = handles.begin(); iter != handles.end(); ++iter)
        {
            push_thread(*iter);
        }
        //
        near == NULL ? wait() :wait(&near->happentime());
    }
}

Timer* TimeLoop::complete_times(std::list<Timer*> &handles)
{
    AUTO_LOCK(&time_locked);
    Timer* near = NULL;
    std::list<Timer*>::iterator iter;
    for(iter = timers.begin();iter!=timers.end();)
    {
        Timer* time = *iter;
        if(Timer::TIME_COMPLETE(time))
        {
            timers.erase(iter++);
            time->UnStart();
            handles.push_back(time);
            //在这里是受保护的
            //push_thread(time);
        }else{
            if(near == NULL)
            {
                near = time;
            }else if(Timer::TIME_EXCEED(near, time)){
                near = time;
            }
            iter++;
        }
    }
    return near;
}

//private
bool TimeLoop::add_time(Timer* time)
{
    AUTO_LOCK(&time_locked);
    std::list<Timer*>::iterator iter = std::find(timers.begin(), timers.end(), time);
    if(iter == timers.end())
    {
        timers.push_back(time);
        time->Reset();
        return true;
    }
    //trace("添加计时器失败");
    return false;
}

bool TimeLoop::remove_time(Timer* time)
{
    AUTO_LOCK(&time_locked);
    std::list<Timer*>::iterator iter = std::find(timers.begin(), timers.end(), time);
    if(iter != timers.end())
    {
        timers.erase(iter);
        return true;
    }
    return false;
}

//public
bool TimeLoop::PushTime(Timer* time)
{
    bool isadd = add_time(time);
    if(isadd) resume();
    return isadd;
}

void TimeLoop::StopTime(Timer* time)
{
    bool isrem = remove_time(time);
    if(isrem) resume();
}
