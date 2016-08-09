//
//  task_scheduler.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/31.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef task_scheduler_h
#define task_scheduler_h

#include <stdio.h>
#include <list>

#include "global.h"
#include "string_utils.h"
#include "task_loop.h"

//最多20个任务线程
#define MAX_TASK_THREAD 20

POWDER_BEGIN

int PushMain(Task* task);

//异步推动一个任务
int AsynPush(Task* task, int tid = 0);

//摧毁一个线程 主现场不能删除
int DelThread(int tid);

POWDER_END

#endif /* task_scheduler_h */
