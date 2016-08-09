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

namespace Log
{
    void setLogLevel(LOG_LEVEL level);
    void debug(const char* fm, ...);
    void info(const char* fm, ...);
    void warn(const char* fm, ...);
    void error(const char* fm, ...);
};


#endif /* Log_hpp */
