//
//  time_utils.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef time_utils_h
#define time_utils_h

#include <sys/time.h>
#include <time.h>

#include "global.h"

//时间搓，秒
typedef time_t TIME_T;
//时间搓，毫秒
typedef clock_t CLOCK_T;

//秒
POWDER_BEGIN
    namespace stime
    {
        //获取当前时间戳
        TIME_T gettime();
        
        TIME_T overtime();
        
        //获取系统经过的时间戳,准确到秒
        TIME_T runtime();
    }

//毫秒
#define MVAL_TIME 1000

    namespace mtime
    {
        //获取当前时间戳
        clock_t gettime();
        
        //获取系统经过的时间戳,准确到秒
        double runtime();
    }

//微妙
#define UVAL_TIME 1000000

    namespace utime
    {
        //当前时间
        struct timeval* gettime(struct timeval *t);
        
        struct timeval* addtime(struct timeval *t, int sec, long usec);
        
        //获取时间差
        int subtime(struct timeval* result, struct timeval* sub, struct timeval* head);
    }

//纳秒
#define NVAL_TIME 1000000000

    namespace ntime
    {
        struct timespec* gettime(struct timespec *t);
        
        struct timespec* addtime(struct timespec *t, int sec, long nsec);
        
        int subtime(struct timespec* result, struct timespec* sub, struct timespec* head);
    }

POWDER_END


class Timeout
{
public:
    Timeout();
    
    Timeout(const char* str, size_t size = 0);
    
    virtual ~Timeout();
private:
    struct timeval _t1;
    const char* _out;
    size_t size;
};

#endif /* time_utils_h */
