//
//  main_loop.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/12.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef main_loop_h
#define main_loop_h

#include "global.h"
#include "lock.h"
#include "task_loop.h"

//class Rock;
//class RockManager;

POWDER_BEGIN

void RunMain(Task* task);
void RunMain(int type, TASK_CALL func, void* args);
void RunMain(int type, TASK_CALL func);
void RunMain(TASK_CALL func, void* args);
void RunMain(TASK_CALL func);

POWDER_END


class RunLoop : public TaskLoop
{
public:
    RunLoop(THREAD_PROXY_FUNC func, bool isrun);
    
    virtual ~RunLoop();
    
};


#endif /* main_loop_h */
