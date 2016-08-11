//
//  timer.hpp
//  ServerPlugIn
//  他不能作为中介传递
//  Created by MikeRiy on 16/7/31.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef timer_h
#define timer_h

#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <algorithm>

#include "time_loop.h"
#include "time_utils.h"

//计时器时间
typedef double delay_t;

class TimeDelegate
{
public:
    virtual void perform(int type)=0;
};

class Timer
{
private:
    delay_t delay_time;
    bool is_time;
    //代理设置
    TimeDelegate* delegate;
    int type;
private:
    struct timespec last;

public:
    Timer();
    
    virtual ~Timer();
    
    virtual void setDelegate(TimeDelegate* delegate, int type = 0);
    
    virtual void setDelay(delay_t delay);
    
    virtual bool start();
    
    virtual void stop();
    
    virtual bool isRunning()const;
    
    //外部不要调用
    friend class TimeLoop;
    struct timespec& happentime();
    //取消
    friend class TimeLoop;
    virtual void UnStart();
    //计时器
    friend class TimeLoop;
    virtual void Reset();
    //推送到任务
    friend class TimeLoop;
    virtual void PushTask();
    
public:
    //时间到达
    static bool TIME_COMPLETE(Timer* time);
    
    //前面超过后面的时间
    static bool TIME_EXCEED(Timer* timer, Timer* other);
};

#endif /* timer_hpp */
