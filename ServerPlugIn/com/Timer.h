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
#include "PowEngine.h"
#include "time_loop.h"
#include "ticker.h"

class TimeManager;

class Timer
{
private:
    friend class TimeLoop;
    uint32 time_id;
    friend class TimeLoop;
    SYS_TIME_T delay_time;
    friend class TimeLoop;
    int m_type;
    friend class TimeLoop;
    ITickHandler* m_event;
public:
    Timer();
    
    Timer(SYS_TIME_T value);
    
    Timer(SYS_TIME_T value, ITickHandler* event, int type = 0);
    
    virtual ~Timer();
    
public:
    virtual void setDelay(SYS_TIME_T value);
    
    virtual void setEventHandler(ITickHandler* event, int type = 0);
    
    virtual void Start();
    
    virtual void Stop();
    
    virtual bool isRunning()const;
    
private:
    friend class TimeLoop;
    void UnStart();
};




#endif /* timemanager_h */
