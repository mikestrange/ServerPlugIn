//
//  thread.h
//  ServerPlugIn
//  线程
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef thread_h
#define thread_h

#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include "lock.h"


class Thread;

//线程回执函数
typedef void(*THREAD_PROXY_FUNC)(Thread *thread);


class Thread
{
private:
    volatile bool is_start;
    volatile bool is_awake;
    volatile bool is_change;
    volatile bool is_run;
public:
    std::string name;
public:
    //通知(不给其他处理就重写perform)
    THREAD_PROXY_FUNC method;
private:
    pthread_t pid_t;
    pthread_cond_t cond_t;
    Locked lock;
public:
    Thread();
    
    Thread(THREAD_PROXY_FUNC func);
    
    virtual ~Thread();
    
    virtual bool start();
    
    virtual void join();
    
    virtual void resume();
    
    virtual void stop();
    
    virtual bool isRunning()const;
    
    virtual void wait(struct timespec* time = NULL);
    
    virtual void kill();
    
private:
    void begin();
    
    void over();
    
protected:
    virtual void perform();
    
    virtual void run();
    
private:
    //thread handle
    static void* ThreadHandle(void *target);
    
public:
    //无参数传递
    static Thread* launch(THREAD_PROXY_FUNC func, const char* name = NULL)
    {
        auto thread = new Thread(func);
        thread->name = name;
        thread->start();
        return thread;
    }
    
};


#endif /* thread_h */
