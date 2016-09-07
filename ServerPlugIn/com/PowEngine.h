//
//  PowEngine.h
//  ServerPlugIn
//   服务器    "火药引擎"
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef PowEngine_h
#define PowEngine_h

#include "global.h"
#include "task_queue.h"
#include "thread.h"
#include "time_loop.h"
#include "Timer.h"

class Timer;

POWDER_BEGIN

void PushTask(Task* task);

void ResumeMain();

//非静态
template<class Class, typename ParamType1, typename ParamType2, typename ParamType3>
void PushMain(Class* obj, void(Class::* func)(ParamType1, ParamType2, ParamType3),
              ParamType1 arg1, ParamType2 arg2, ParamType3 arg3)
{
    PushTask(new Block::Callback<Class, ParamType1, ParamType2, ParamType3>(obj, func, arg1, arg2, arg3));
}

template<class Class, typename ParamType1, typename ParamType2>
void PushMain(Class* obj, void(Class::* func)(ParamType1, ParamType2), ParamType1 arg1, ParamType2 arg2){
    PushTask(new Block::Callback<Class, ParamType1, ParamType2>(obj, func, arg1, arg2));
}

template<class Class, typename ParamType1>
void PushMain(Class* obj, void(Class::* func)(ParamType1), ParamType1 arg1)
{
    PushTask(new Block::Callback<Class, ParamType1>(obj, func, arg1));
}

template<class Class>
void PushMain(Class* obj, void(Class::* func)(void))
{
    PushTask(new Block::Callback<Class>(obj, func));
}

//静态
template<typename ParamType1, typename ParamType2, typename ParamType3>
void SPushMain(void(*func)(ParamType1, ParamType2, ParamType3), ParamType1 arg1, ParamType2 arg2, ParamType3 arg3)
{
    PushTask(new Block::SCallback<ParamType1, ParamType2, ParamType3>(func, arg1, arg2, arg3));
}

template<typename ParamType1, typename ParamType2>
void SPushMain(void(*func)(ParamType1, ParamType2), ParamType1 arg1, ParamType2 arg2)
{
    PushTask(new Block::SCallback<ParamType1, ParamType2>(func, arg1, arg2));
}

template<typename ParamType1>
void SPushMain(void(*func)(ParamType1), ParamType1 arg1)
{
    PushTask(new Block::SCallback<ParamType1>(func, arg1));
}


POWDER_END

#endif /* PowEngine_h */
