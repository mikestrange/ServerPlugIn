//
//  task_loop.h
//  ServerPlugIn
//  任务池(被事件调度)
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef task_loop_h
#define task_loop_h

#include <stdio.h>
#include <list>
#include "thread.h"
#include "task.h"
#include "global.h"
#include "lock.h"

class TaskLoop : public Thread
{
private:
    std::list<Task*> tasks;
    Locked lock;
    
public:
    TaskLoop(){};
    TaskLoop(THREAD_PROXY_FUNC func);
    virtual ~TaskLoop();
    
public:
    //推送任务
    virtual bool PushTask(Task* task);
    //轮询任务
    virtual bool PollTask();
    
private:
    bool AddTask(Task* task);
    
    Task* Next();
    
    void Clean();
};

#endif /* task_loop_h */
