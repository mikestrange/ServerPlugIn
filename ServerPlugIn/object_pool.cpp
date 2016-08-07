//
//  object_loop.hpp
//  ServerPlugIn
//  对象池
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "object_pool.h"


ByteArrayPool* ByteArrayPool::_instance = NULL;

ByteArrayPool* ByteArrayPool::getInstance()
{
    if(NULL == _instance)
    {
        _instance = new ByteArrayPool();
        trace("初始化缓存池");
    }
    return _instance;
}