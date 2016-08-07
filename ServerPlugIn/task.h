//
//  task_base.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef task_base_h
#define task_base_h

#include <stdio.h>

//args为内存(必须为静态回调)
typedef void(*TASK_CALL)(int type, void* args);

class Task
{
public:
    Task(int _type, TASK_CALL _func, void* _args = NULL)
    :type(_type)
    ,func(_func)
    ,args(_args)
    {}
    
    virtual ~Task()
    {
    
    }
    
    virtual void perform()
    {
        func(type, args);
    }
    
    virtual void destroy()
    {
    
    }
    
public:
    static Task* create(int type, TASK_CALL func, void* args)
    {
        return new Task(type, func, args);
    }
    
    static Task* create(int type, TASK_CALL func)
    {
        return new Task(type, func, NULL);
    }
    
    static Task* create(TASK_CALL func, void* args)
    {
        return new Task(0, func, args);
    }
    
    static Task* create(TASK_CALL func)
    {
        return new Task(0, func, NULL);
    }
    
private:
    int type;
    TASK_CALL func;
    void* args;
};

#endif /* task_base_h */
