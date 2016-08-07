//
//  object_loop.hpp
//  ServerPlugIn
//  对象池
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef object_loop_h
#define object_loop_h

#include <stdio.h>
#include <list>

#include "lock.h"
#include "byte_array.h"

#define MAX_POOL_LEN 1024

template<class T>
class ObjectPool
{
private:
    int _maxLen = 0;
private:
    std::list<T*> objects;
    Locked lock;
public:
    ObjectPool()
    :_maxLen(MAX_POOL_LEN)
    {}
    
    ObjectPool(int maxLen)
    :_maxLen(maxLen)
    {}
    
    virtual ~ObjectPool()
    {
        clean();
    }
    
public:
    //获取一个
    virtual T* peel()
    {
        AUTO_LOCK(&lock);
        if(objects.empty()) return NULL;
        T* obj = objects.back();
        objects.pop_back();
        return obj;
    }
    
    virtual bool share(T* obj)
    {
        AUTO_LOCK(&lock);
        if(objects.size()>=_maxLen)
        {
            return false;
        }
        objects.push_back(obj);
        return true;
    }
    
    virtual void clean()
    {
        AUTO_LOCK(&lock);
        typename std::list<T*>::iterator iter;
        for(iter=objects.begin();iter!=objects.end();++iter)
        {
            SAFE_DELETE(*iter);
        }
        objects.clear();
    }
};


class ByteArrayPool : public ObjectPool<ByteArray>
{
private:
    static ByteArrayPool* _instance;
public:
    static ByteArrayPool* getInstance();
};


#endif /* object_loop_hpp */
