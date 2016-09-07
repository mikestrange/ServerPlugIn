//
//  PowEngine.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "PowEngine.h"

static void main_handle(Thread* thread);
//直接启动
static Thread* const mainLoop = Thread::launch(&main_handle, "Main Thread");
//任务列表
static TaskQueue* const tasks = new TaskQueue();

//主线程处理
static void main_handle(Thread* thread)
{
    Log::info("//*********主线程开始*********\\");
    struct timespec delay_t;
    while(thread->isRunning())
    {
        //所有及时任务先进行
        if(tasks->DispatchTask()) continue;
        //trace("*********wait for main*********");
        SYS_TIME_T delay = TimeLoop::getInstance()->RunLoop();
        powder::transform_timespec(&delay_t, delay);
        //trace("time = %ld",delay);
        thread->wait(delay > 0 ? &delay_t : NULL);
        //sleep(1);
    }
    Log::warn("//*********主线程断掉了*********\\");
}


POWDER_BEGIN

void PushTask(Task* task)
{
    if(tasks->AppendTask(task))
    {
        mainLoop->resume();
    };
}


void ResumeMain()
{
    mainLoop->resume();
}
POWDER_END