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
    Log::info("//*********主线程开始*********\\");
    while(loop->isRunning())
    {
        //所有及时任务先进行
        if(loop->PollTask()) continue;
        //计时器派送
        loop->wait();
        trace("*********run main*********");
    }
    Log::info("//*********主线程断掉了*********\\");
    SAFE_DELETE(thread);
}


//直接启动
static RunLoop* const mainLoop = new RunLoop(&main_handle, true);


POWDER_BEGIN

void RunTask(Task* task)
{
    mainLoop->PushTask(task);
}

//静态
template<typename ParamType1, typename ParamType2, typename ParamType3>
void SRunMain(void(*func)(ParamType1, ParamType2, ParamType3), ParamType1 arg1, ParamType2 arg2, ParamType3 arg3)
{
    RunTask(new Block::SCallback<ParamType1, ParamType2, ParamType3>(func, arg1, arg2, arg3));
}

template<typename ParamType1, typename ParamType2>
void SRunMain(void(*func)(ParamType1, ParamType2), ParamType1 arg1, ParamType2 arg2)
{
    RunTask(new Block::SCallback<ParamType1, ParamType2>(func, arg1, arg2));
}

template<typename ParamType1>
void SRunMain(void(*func)(ParamType1), ParamType1 arg1)
{
    RunTask(new Block::SCallback<ParamType1>(func, arg1));
}

void SRunMain(void(*func)(void))
{
    RunTask(new Block::SCallback<>(func));
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