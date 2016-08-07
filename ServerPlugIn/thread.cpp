//
//  thread.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "thread.h"

Thread::Thread()
:is_start(false)
,is_change(false)
,is_run(false)
,method(NULL)
{
    pthread_cond_init(&cond_t, 0);
}

Thread::Thread(THREAD_PROXY_FUNC func, bool isrun, const char* name)
:is_start(false)
,is_change(false)
,is_run(false)
,name(name)
,method(func)
{
    pthread_cond_init(&cond_t, 0);
    if(isrun) start();
}

Thread::~Thread()
{
    pthread_cond_destroy(&cond_t);
    //kill();
    trace("~Thread Release: %s", name.c_str());
}

bool Thread::start()
{
    if(is_start) return false;
    //create thread
    int code = pthread_create(&pid_t, 0, &Thread::ThreadHandle, this);
    if(code == 0)
    {
        is_start = true;
        return true;
    }
    return false;
}

void Thread::join()
{
    if(is_start)
    {
        pthread_join(pid_t, 0);
    }
}

void Thread::stop()
{
    is_run = false;
    //如果线程关闭了，必须唤醒
    resume();
}

bool Thread::isRunning()const
{
    return is_run;
}

void Thread::kill()
{
    int kill_ret = pthread_kill(pid_t, 0);
    if(kill_ret == ESRCH){
        trace("kill is no thread:%s", name.c_str());
    }else if(kill_ret == EINVAL){
        trace("kill is no user sign:%s", name.c_str());
    }else {
        trace("kill ok:%s", name.c_str());
    }
}

void Thread::resume()
{
    //错误也能返回
    AUTO_LOCK(&lock);
    if(!is_change)
    {
        //is_change = true;
        if(name.length() > 0)  trace("start resume thread: %s", name.c_str());
        pthread_cond_signal(&cond_t);
    }
}

void Thread::wait(struct timespec* time)
{
    AUTO_LOCK(&lock);
    if(is_change)
    {
        is_change = false;
        if(name.length() > 0) trace("start wait thread: %s", name.c_str());
        if(time){
            pthread_cond_timedwait(&cond_t, lock.getLocked(), time);
        }else{
            pthread_cond_wait(&cond_t, lock.getLocked());
        }
        is_change = true;
    }
}

//private
void Thread::begin()
{
    is_start = true;
    is_change = false;
    is_run = true;
    perform(THREAD_BEGIN);
}

void Thread::over()
{
    is_start = false;
    is_change = false;
    is_run = false;
    perform(THREAD_OVER);
}


//protected
void Thread::perform(int type)
{
    if(method)
    {
        method(type, this);
    }
}

void Thread::run()
{
    perform(THREAD_RUN);
}

//private static
void* Thread::ThreadHandle(void *target)
{
    Thread *thread = (Thread*)target;
    thread->begin();
    thread->run();
    thread->over();
    return 0;
}