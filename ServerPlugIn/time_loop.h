//
//  time_loop.hpp
//  ServerPlugIn
//  所有计时器都到这里，但是执行不在这里
//  Created by MikeRiy on 16/7/31.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef time_loop_h
#define time_loop_h

#include <stdio.h>
#include <list>

#include "thread.h"
#include "timer.h"
#include "global.h"
#include "task_scheduler.h"

class Timer;

class TimeLoop : public Thread
{
protected:
    void run()override;
private:
    Timer* complete_times(std::list<Timer*> &handles);
private:
    bool add_time(Timer* time);
    bool remove_time(Timer* time);
public:
    bool PushTime(Timer* time);
    void StopTime(Timer* time);
    
private:
    std::list<Timer*> timers;
    Locked time_locked;
    static TimeLoop* _instance;
public:
    static TimeLoop* getInstance();
};


#endif /* time_loop_h */
