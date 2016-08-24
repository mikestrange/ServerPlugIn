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


POWDER_BEGIN

void RunTask(Task* task);

//非静态
template<class Class, typename ParamType1, typename ParamType2, typename ParamType3>
void RunMain(Class* obj, void(Class::* func)(ParamType1, ParamType2, ParamType3),
             ParamType1 arg1, ParamType2 arg2, ParamType3 arg3)
{
    RunTask(new Block::Callback<Class, ParamType1, ParamType2, ParamType3>(obj, func, arg1, arg2, arg3));
}

template<class Class, typename ParamType1, typename ParamType2>
void RunMain(Class* obj, void(Class::* func)(ParamType1, ParamType2), ParamType1 arg1, ParamType2 arg2){
    RunTask(new Block::Callback<Class, ParamType1, ParamType2>(obj, func, arg1, arg2));
}

template<class Class, typename ParamType1>
void RunMain(Class* obj, void(Class::* func)(ParamType1), ParamType1 arg1)
{
    RunTask(new Block::Callback<Class, ParamType1>(obj, func, arg1));
}

template<class Class>
void RunMain(Class* obj, void(Class::* func)(void))
{
    RunTask(new Block::Callback<Class>(obj, func));
}

//静态
template<typename ParamType1, typename ParamType2, typename ParamType3>
void SRunMain(void(*func)(ParamType1, ParamType2, ParamType3), ParamType1 arg1, ParamType2 arg2, ParamType3 arg3);

template<typename ParamType1, typename ParamType2>
void SRunMain(void(*func)(ParamType1, ParamType2), ParamType1 arg1, ParamType2 arg2);

template<typename ParamType1>
void SRunMain(void(*func)(ParamType1), ParamType1 arg1);

void SRunMain(void(*func)(void));

POWDER_END


class RunLoop : public TaskLoop
{
public:
    RunLoop(THREAD_PROXY_FUNC func, bool isrun);
    
    virtual ~RunLoop();
    
};


#endif /* main_loop_h */
