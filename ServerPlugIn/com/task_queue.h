//
//  task_queue.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/26.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef task_queue_hpp
#define task_queue_hpp


#include <list>

#include "global.h"
#include "task.h"
#include "lock.h"

class TaskQueue
{
public:
    TaskQueue();
    virtual ~TaskQueue();
    
public:
    virtual bool AppendTask(Task* task);
    
    virtual bool DispatchTask();
    
    virtual void Clean();
    
private:
    virtual Task* GetNext();
private:
    std::list<Task*> tasks;
    Locked lock;
};

#endif /* task_queue_hpp */
