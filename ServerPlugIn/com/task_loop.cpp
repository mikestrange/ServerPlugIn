//
//  event_loop.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "task_loop.h"

TaskLoop::TaskLoop(THREAD_PROXY_FUNC func)
:Thread(func)
{}


TaskLoop::~TaskLoop()
{
    Clean();
}

//public
bool TaskLoop::PushTask(Task *task)
{
    if(AddTask(task))
    {
        resume();
        return true;
    }
    return false;
}

bool TaskLoop::PollTask()
{
    Task* task = Next();
    if(task)
    {
        task->perform();
        SAFE_DELETE(task);
        return true;
    }
    return false;
}

//private
bool TaskLoop::AddTask(Task* task)
{
    AUTO_LOCK(&lock);
    if(tasks.size() >= tasks.max_size())
    {
        return false;
    }
    tasks.push_back(task);
    return true;
}

void TaskLoop::Clean()
{
    AUTO_LOCK(&lock);
    std::list<Task*>::iterator iter;
    for(iter = tasks.begin();iter != tasks.end(); ++iter)
    {
        auto task = *iter;
        task->destroy();
        SAFE_DELETE(task);
    }
    tasks.clear();
}

Task* TaskLoop::Next()
{
    AUTO_LOCK(&lock);
    if(tasks.empty())
    {
        return NULL;
    }
    Task* task = tasks.front();
    tasks.pop_front();
    return task;
}

//void TaskLoop::run()
//{
//    while(isRunning())
//    {
//        if(!Poll())
//        {
//            wait();
//        }
//        //printf("[dispatch loop handle ...]\n");
//    }
//}