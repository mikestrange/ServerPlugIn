//
//  Log.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef Log_h
#define Log_h

#include <stdlib.h>
#include <string>
#include "global.h"

typedef enum
{
    LOG_NONE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
}LOG_LEVEL;

#define LOG_SIZE    512
#define TIME_SIZE   64

class Log
{
private:
    //默认打印等级
    static const LOG_LEVEL logLevel = LOG_NONE;
    
private:
    static void output(const char* log,const char* type)
    {
        time_t t = time(nullptr);
        char tmp[TIME_SIZE];
        strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));
        trace("%s[%s] %s", tmp, type, log);
        //统一的保存
    }
    
public:
    static void debug(const char* fm, ...)
    {
        if(logLevel > LOG_DEBUG) return;
        STR_FORMAT(LOG_SIZE);
        output(buf,"DEBUG");
    }
    
    static void info(const char* fm, ...)
    {
        if(logLevel > LOG_INFO) return;
        STR_FORMAT(LOG_SIZE);
        output(buf,"INFO");
    }
    
    static void warn(const char* fm, ...)
    {
        if(logLevel > LOG_WARN) return;
        STR_FORMAT(LOG_SIZE);
        output(buf,"WARN");
    }
    
    static void error(const char* fm, ...)
    {
        if(logLevel > LOG_ERROR) return;
        STR_FORMAT(LOG_SIZE);
        output(buf,"ERROR");
    }
};


#endif /* Log_hpp */
