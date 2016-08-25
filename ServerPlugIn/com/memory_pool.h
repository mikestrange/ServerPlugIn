//
//  memory_pool.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/11.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef memory_pool_h
#define memory_pool_h

#include "global.h"
#include "lock.h"

#include <vector>

#define MAX_MEMORY_LEN 2048

#define NEW_CHAR_LEN 1024

class MemoryPool
{
private:
    int _maxLen = 0;
private:
    std::vector<char*> pools;
    Locked lock;
public:
    MemoryPool()
    :_maxLen(MAX_MEMORY_LEN)
    {}
    
    MemoryPool(int maxLen)
    :_maxLen(maxLen)
    {}
    
    virtual ~MemoryPool()
    {
        clean();
    }
    
public:
    //获取一个
    virtual char* peel()
    {
        AUTO_LOCK(&lock);
        if(pools.empty())
        {
            return new char[NEW_CHAR_LEN];
        }
        char* bytes = pools.back();
        pools.pop_back();
        return bytes;
    }
    
    virtual bool share(char* obj)
    {
        AUTO_LOCK(&lock);
        if(pools.size() >= _maxLen)
        {
            delete obj;
            return false;
        }
        pools.push_back(obj);
        return true;
    }
    
    virtual void clean()
    {
        AUTO_LOCK(&lock);
        typename std::vector<char*>::iterator iter;
        for(iter=pools.begin();iter!=pools.end();++iter)
        {
            SAFE_DELETE(*iter);
        }
        pools.clear();
    }
    
    virtual char* alloc_copy(char* src, size_t size)
    {
        char* chars = peel();
        memcpy(chars, src, size);
        return chars;
    }
    
private:
    static MemoryPool* _instance;
public:
    static MemoryPool* getInstance();
};

#endif /* memory_pool_h */
