//
//  lock.h
//  ServerPlugIn
//  线程锁 基础
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef lock_h
#define lock_h

#include <stdio.h>
#include <pthread.h>

#include "global.h"

#define AUTO_LOCK(key)             AutoLocked __am(key);
#define AUTO_LOCKS(key, byte)      AutoLocked __am(key, byte);

class Locked
{
public:
    Locked();
    
    virtual ~Locked();
    
    virtual void lock();
    
    virtual void unlock();
    
    virtual pthread_mutex_t* getLocked();
    //属性
protected:
    pthread_mutex_t m_lock;
    volatile bool is_locked;
};


//自动线程锁
class AutoLocked
{
private:
    Locked* _mutex;
    const char* _buffer;
public:
    AutoLocked(Locked *mutex)
    :_mutex(mutex)
    ,_buffer(NULL)
    {
        _mutex->lock();
    }
    
    AutoLocked(Locked *mutex, const char* buffer)
    :_mutex(mutex)
    ,_buffer(buffer)
    {
        _mutex->lock();
        if(buffer)
        {
            trace("lock : %s",_buffer);
        }
    }
    
    virtual ~AutoLocked()
    {
        _mutex->unlock();
        _mutex = NULL;
        if(_buffer)
        {
            trace("unlock : %s",_buffer);
            _buffer = NULL;
        }
    }
};


#endif /* lock_h */
