//
//  rock_manager.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/12.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef rock_manager_h
#define rock_manager_h

#include "Timer.h"
#include "thread.h"
#include "main_loop.h"

class Timer;

//计时器管理(当前线程使用)
class TimeManager : public Thread
{
private:
    int curRockId = 0;
    std::map<int, Timer*> rockMap;
    Locked rockLock;
private:
    int create_Id();
public:
    TimeManager();
    virtual ~TimeManager();
    virtual bool PushTimer(Timer* rock);
    virtual bool EndTimer(Timer* rock);
    virtual void CleanTimers();
    //virtual bool HandleTimers(struct timespec &delay);
private:
    virtual void HandleTimer(int rockid);
protected:
    void run()override;
private:
    static TimeManager* _instance;
public:
    static TimeManager* getInstance();
public:
    static bool TIME_COMPLETE(struct timespec &now, Timer* time);
    static bool TIME_EXCEED(Timer* timer, Timer* other);
};



#endif /* rock_manager_hpp */
