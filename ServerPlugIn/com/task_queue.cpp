//
//  task_queue.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/26.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "task_queue.h"


TaskQueue::TaskQueue()
{}


TaskQueue::~TaskQueue()
{
    Clean();
}

bool TaskQueue::DispatchTask()
{
    Task* task = GetNext();
    if(task)
    {
        task->perform();
        SAFE_DELETE(task);
        return true;
    }
    return false;
}

//private
bool TaskQueue::AppendTask(Task* task)
{
    AUTO_LOCK(&lock);
    if(tasks.size() >= tasks.max_size())
    {
        task->destroy();
        SAFE_DELETE(task);
        return false;
    }
    tasks.push_back(task);
    return true;
}

void TaskQueue::Clean()
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

Task* TaskQueue::GetNext()
{
    AUTO_LOCK(&lock);
    if(tasks.empty()) return NULL;
    //next
    Task* task = tasks.front();
    tasks.pop_front();
    return task;
}