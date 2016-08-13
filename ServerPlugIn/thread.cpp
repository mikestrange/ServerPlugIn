//
//  thread.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "thread.h"

Thread::Thread()
:pid_t(0)
,is_awake(true)
,is_change(false)
,is_run(true)
,method(NULL)
{
    pthread_cond_init(&cond_t, 0);
}

Thread::Thread(THREAD_PROXY_FUNC func)
:pid_t(0)
,is_awake(true)
,is_change(false)
,is_run(true)
,method(func)
{
    pthread_cond_init(&cond_t, 0);
}

Thread::~Thread()
{
    pthread_cond_destroy(&cond_t);
    kill();
    trace("~Thread Release: %s", name.c_str());
}

bool Thread::start()
{
    if(pid_t > 0){
        trace("please kill first");
        return false;
    }
    int code = pthread_create(&pid_t, 0, &Thread::ThreadHandle, this);
    if(code != 0)
    {
        trace("run thread error");
    }
    //create thread
    return code == 0;
}

void Thread::join()
{
    pthread_join(pid_t, 0);
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
    if(pid_t > 0){
        int ret = pthread_cancel(pid_t);
        pid_t = 0;
        trace("cance thread:%d", ret);
    }
}

void Thread::resume()
{
    //错误也能返回
    AUTO_LOCK(&lock);
    is_change = true;
    if(is_awake == false)
    {
        is_awake = true;
        //if(name.length() > 0)  trace("start resume thread: %s", name.c_str());
        pthread_cond_signal(&cond_t);
    }
}

void Thread::wait(struct timespec* time)
{
    AUTO_LOCK(&lock);
    if(is_awake)
    {
        if(is_change)
        {
            is_change = false;
        }else{
            is_awake = false;
            //if(name.length() > 0) trace("start wait thread: %s", name.c_str());
            if(time){
                pthread_cond_timedwait(&cond_t, lock.getLocked(), time);
            }else{
                pthread_cond_wait(&cond_t, lock.getLocked());
            }
            is_awake = true;
        }
    }
}

//protected
void Thread::perform()
{
    if(method) method(this);
}

void Thread::run()
{
    perform();
}

//private static
void* Thread::ThreadHandle(void *target)
{
    Thread *thread = (Thread*)target;
    thread->run();
    return 0;
}