//
//  main_loop.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/12.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "main_loop.h"

//主线程处理
static void main_handle(Thread* thread)
{
    RunLoop* loop = dynamic_cast<RunLoop*>(thread);
    trace("//*********主线程开始*********\\");
    while(loop->isRunning())
    {
        //所有及时任务先进行
        if(loop->PollTask()) continue;
        //计时器派送
        loop->wait();
        trace("*********run main*********");
    }
    trace("//*********主线程断掉了*********\\");
    SAFE_DELETE(thread);
}


//直接启动
static RunLoop* const mainLoop = new RunLoop(&main_handle, true);


POWDER_BEGIN

void RunMain(Task* task)
{
    mainLoop->PushTask(task);
}

void RunMain(int type, TASK_CALL func, void* args)
{
    RunMain(Task::create(type, func, args));
}

void RunMain(int type, TASK_CALL func)
{
     RunMain(Task::create(type, func));
}

void RunMain(TASK_CALL func, void* args)
{
     RunMain(Task::create(func, args));
}

void RunMain(TASK_CALL func)
{
     RunMain(Task::create(func));
}

POWDER_END


//class
RunLoop::RunLoop(THREAD_PROXY_FUNC func, bool isrun)
:TaskLoop(func)
{
    if(isrun) start();
}

RunLoop::~RunLoop(){
    
}