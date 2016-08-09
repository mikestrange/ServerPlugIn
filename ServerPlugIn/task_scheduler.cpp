//
//  task_scheduler.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/31.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "task_scheduler.h"

static bool is_init = false;
static TaskLoop* taskLoops[MAX_TASK_THREAD];
static Locked locker;

//初始化
static void init_loops()
{
    if(!is_init)
    {
        trace("init task loops");
        is_init = true;
        for(int i = 0; i < MAX_TASK_THREAD; i++)
        {
            taskLoops[i] = NULL;
        }
    }
}

//任务线程处理
static void thread_handle(int type, Thread* thread)
{
    TaskLoop* task = dynamic_cast<TaskLoop*>(thread);
    if(type == THREAD_BEGIN){
        trace("start task thread");
    }else if(type == THREAD_OVER){
        trace("delete task thread");
        SAFE_DELETE(thread);
    }else{
        while(thread->isRunning())
        {
            if(task->PollTask()) continue;
            //暂停
            task->wait();
        }
    }
}

POWDER_BEGIN


int PushMain(Task* task)
{
    return AsynPush(task, 0);
}

int AsynPush(Task* task, int tid)
{
    if(tid < 0 || tid >= MAX_TASK_THREAD)
    {
        return -1;
    }
    //
    AUTO_LOCK(&locker);
    init_loops();
    //
    TaskLoop* loop = taskLoops[tid];
    if(loop == NULL)
    {
        loop = new TaskLoop(&thread_handle);
        if(loop->start())
        {
            loop->name = StringUtil::format("Task %d", tid);
            taskLoops[tid] = loop;
            loop->PushTask(task);
            trace("##Run Task Thread: %d",tid);
            return 0;
        }else{
            SAFE_DELETE(loop);
        }
    }else{
        loop->PushTask(task);
    }
    return -1;
}

int DelThread(int tid)
{
    if(tid > 0 && tid < MAX_TASK_THREAD)
    {
        AUTO_LOCK(&locker);
        TaskLoop* loop = taskLoops[tid];
        if(loop)
        {
            taskLoops[tid] = NULL;
            loop->stop();
            return 0;
        }
    }
    return -1;
}

POWDER_END