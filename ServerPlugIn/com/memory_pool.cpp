//
//  memory_pool.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/11.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "memory_pool.h"


MemoryPool* MemoryPool::_instance = NULL;

MemoryPool* MemoryPool::getInstance()
{
    if(NULL == _instance)
    {
        _instance = new MemoryPool();
        trace("初始化缓存池");
    }
    return _instance;
}