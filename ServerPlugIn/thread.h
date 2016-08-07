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

//线程回执事件
typedef enum _THREAD_EVENT
{
    THREAD_BEGIN = 1,  //用于初始化对象
    THREAD_RUN,        //用于处理
    THREAD_OVER,       //用于删除对象
}THREAD_EVENT;

//线程函数
//typedef void *(*THREAD_CALL)(void *);
//线程回执函数
typedef void(*THREAD_PROXY_FUNC)(int type, Thread *thread);


class Thread
{
private:
    volatile bool is_start;
    volatile bool is_change;
    volatile bool is_run;
public:
    //线程名称，无很大作用
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
    
    Thread(THREAD_PROXY_FUNC func, bool isrun = false, const char* name= NULL);
    
    virtual ~Thread();
    
    virtual bool start();
    
    virtual void join();
    
    virtual void resume();
    
    virtual void kill();
    
    virtual void stop();
    
    virtual bool isRunning()const;
    
    virtual void wait(struct timespec* time = NULL);
    
private:
    void begin();
    
    void over();
    
protected:
    virtual void perform(int type);
    
    virtual void run();
    
private:
    //thread handle
    static void* ThreadHandle(void *target);
    
public:
    //无参数传递
    static void launch(THREAD_PROXY_FUNC func, const char* name = NULL)
    {
        new Thread(func, true, name);
    }
};


#endif /* thread_h */
