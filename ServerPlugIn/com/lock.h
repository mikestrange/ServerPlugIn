//
//  lock.h
//  ServerPlugIn
//  线程锁 基础
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef lock_h
#define lock_h

#include <pthread.h>

#include "global.h"

#define AUTO_LOCK(key)             AutoLocked __am(key);

#define OBJ_RETAIN(obj)                 do{ if(obj){ obj->retain(); } }while(0)
#define OBJ_RELEASE(obj)                do{ if(obj){ obj->release(); obj = NULL;} }while(0)



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
public:
    AutoLocked(Locked *mutex)
    :_mutex(mutex)
    {
        _mutex->lock();
    }
    
    virtual ~AutoLocked()
    {
        _mutex->unlock();
    }
};

class RefObject
{
private:
    unsigned long m_count;
public:
    RefObject()
    :m_count(1)
    {}
    
    virtual ~RefObject()
    {
        
    }
    
    bool release()
    {
        --m_count;
        if(!m_count)
        {
            delete this;
            return true;
        }
        return false;
    }
    
    void retain()
    {
        ++m_count;
    }
};

#endif /* lock_h */
