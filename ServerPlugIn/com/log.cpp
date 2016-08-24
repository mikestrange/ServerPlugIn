//
//  Log.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "log.h"

static const int LOG_SIZE = 512;
static const int TIME_SIZE = 64;
//默认打印等级
static LOG_LEVEL logLevel = LOG_NONE;

static void output(const char* log,const char* type)
{
    time_t t = time(nullptr);
    char tmp[TIME_SIZE];
    strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));
    printf("%s[%s] %s\n", tmp, type, log);
}

namespace Log
{
    void setLogLevel(LOG_LEVEL level)
    {
        logLevel = level;
    }
    
    void debug(const char* fm, ...)
    {
        if(logLevel > LOG_DEBUG) return;
        STR_FORMAT(LOG_SIZE);
        output(buf,"DEBUG");
    }
    
    void info(const char* fm, ...)
    {
        if(logLevel > LOG_INFO) return;
        STR_FORMAT(LOG_SIZE);
        output(buf,"INFO");
    }
    
    void warn(const char* fm, ...)
    {
        if(logLevel > LOG_WARN) return;
        STR_FORMAT(LOG_SIZE);
        output(buf,"WARN");
    }
    
    void error(const char* fm, ...)
    {
        if(logLevel > LOG_ERROR) return;
        STR_FORMAT(LOG_SIZE);
        output(buf,"ERROR");
    }
}