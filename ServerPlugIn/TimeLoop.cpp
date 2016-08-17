//
//  rock_manager.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/12.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "TimeLoop.h"

TimeManager* TimeManager::_instance = NULL;

TimeManager* TimeManager::getInstance(){
    if(!_instance)
    {
        _instance = new TimeManager;
        trace("<instance timemanager>");
    }
    return _instance;
}

TimeManager::TimeManager()
:curRockId(0)
{
    start();
}

TimeManager::~TimeManager()
{
    CleanTimers();
}

int TimeManager::create_Id()
{
    curRockId++;
    if(curRockId >= INT32_MAX) curRockId = 1;
    if(rTab.has(curRockId))
    {
        return create_Id();
    }
    return curRockId;
}

//class RockManager
bool TimeManager::PushTimer(Timer* rock)
{
    do{
        AUTO_LOCK(&rockLock);
        if(rock->isRunning())
        {
            return false;
        }else{
            int rockid = create_Id();
            rTab.put(rockid, rock);
            rock->Reset(rockid);
        }
    }while(0);
    //唤醒
    resume();
    return true;
}

bool TimeManager::EndTimer(Timer* rock)
{
    do{
        AUTO_LOCK(&rockLock);
        if(rock->isRunning()){
            auto tv = rTab.remove(rock->rockId());
            if(tv)
            {
                tv->rockId(0);
            }else{
                return false;
            }
        }else{
            return false;
        }
    }while(0);
    //唤醒
    resume();
    return true;
}

void TimeManager::HandleTimer(int rockid)
{
    do{
        AUTO_LOCK(&rockLock);
        auto tick = rTab.remove(rockid);
        if(tick)
        {
            tick->rockId(0);
            //派送到主线程序 注意线程安全的delegte
            powder::RunMain(Task::create(tick->call_type, block(int type, void* args){
                TimeOutEvent* delegate = (TimeOutEvent*)args;
                if(delegate)
                {
                    delegate->onTimeoutProcess(type);
                }
            },tick->delegate));
        }
    }while(0);
}

void TimeManager::CleanTimers()
{
    AUTO_LOCK(&rockLock);
    rTab.clear(block(Timer* timer){
        timer->rockId(0);
    });
}


void TimeManager::run()
{
    while(isRunning())
    {
        Timer* near = NULL;
        struct timespec delay;
        powder::ntime::gettime(&delay);
        std::vector<int> handles;
        //get handles
        do{
            AUTO_LOCK(&rockLock);
            HashMap<int, Timer*>::Iterator iter;
            for(iter = rTab.begin();iter!=rTab.end();iter++)
            {
                Timer* rock = iter->second;
                if(TimeManager::TIME_COMPLETE(delay, rock))
                {
                    handles.push_back(iter->first);
                }else{
                    if(near == NULL)
                    {
                        near = rock;
                    }else if(TimeManager::TIME_EXCEED(near, rock)){
                        near = rock;
                    }
                }
            }
            //最后时间
            if(near)
            {
                delay.tv_sec = near->happentime().tv_sec;
                delay.tv_nsec = near->happentime().tv_nsec;
            }
        }while(0);
        //handle rocks
        std::vector<int>::iterator viter;
        for(viter = handles.begin(); viter != handles.end(); ++viter)
        {
            TimeManager::HandleTimer(*viter);
        }
        //stop?
        wait(near ? &delay : NULL);
    }
}


//static others
bool TimeManager::TIME_COMPLETE(struct timespec &now, Timer* time)
{
    struct timespec& last = time->happentime();
    //trace("%ld %ld %ld %ld",now.tv_sec, last.tv_sec,now.tv_nsec,last.tv_nsec);
    if(now.tv_sec > last.tv_sec) return true;
    if(now.tv_sec == last.tv_sec && now.tv_nsec >= last.tv_nsec) return true;
    return false;
}

bool TimeManager::TIME_EXCEED(Timer* timer, Timer* other)
{
    struct timespec& value1 = timer->happentime();
    struct timespec& value2 = other->happentime();
    if(value1.tv_sec > value2.tv_sec) return true;
    if(value1.tv_sec == value2.tv_sec && value1.tv_nsec >= value2.tv_nsec) return true;
    return false;
}