//
//  error.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "error.h"

Error::Error()
:type(0)
,msg(NULL){}

Error::Error(const char* bytes)
:type(0)
{
    Message(bytes);
}

Error::Error(int idx, const char* bytes)
:type(idx)
{
    Message(bytes);
}

Error::~Error()
{
    FREE(msg);
}

void Error::toString()
{
    trace("Error printf: id = %d, msg = %s", type, msg);
}

void Error::Message(const char* bytes)
{
    FREE(msg);
    size_t len = strlen(bytes);
    msg = (char*)malloc(len);
    memcpy(msg, bytes, len);
}

namespace error
{
    void show(const char* fm, ...)
    {
        STR_FORMAT(1024);
        trace("error msg:%s",buf);
        //抛出错误
        throw Error(buf);
    }
    
    void show(int type, const char* fm, ...)
    {
        STR_FORMAT(1024);
        trace("error msg:%s",buf);
        //抛出系统错误
        throw Error(type, buf);
    }
}
