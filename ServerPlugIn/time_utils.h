//
//  time_utils.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef time_utils_hpp
#define time_utils_hpp

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <mach/mach_time.h>
#include "global.h"
//秒
POWDER_BEGIN
    namespace stime
    {
        //获取当前时间戳
        time_t gettime();
        
        time_t overtime();
        
        //获取系统经过的时间戳,准确到秒
        time_t runtime();
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
        
        struct timeval* addtime(struct timeval *t,__darwin_time_t sec,__darwin_suseconds_t usec);
        
        //获取时间差
        int subtime(struct timeval* result, struct timeval* sub, struct timeval* head);
    }

//纳秒
#define NVAL_TIME 1000000000

    namespace ntime
    {
        struct timespec* gettime(struct timespec *t);
        
        struct timespec* addtime(struct timespec *t, __darwin_time_t sec, long nsec);
        
        int subtime(struct timespec* result, struct timespec* sub, struct timespec* head);
    }

POWDER_END


//printf( "The file is %s.\n", __FILE__ );
//printf( "The date is %s.\n", __DATE__ );
//printf( "The time is %s.\n", __TIME__ );
//printf( "This is line %d.\n", __LINE__ );
//printf( "This function is %s.\n", __FUNCTION__ );
class Timeout
{
public:
    Timeout()
    :_out(NULL)
    {
        powder::utime::gettime(&_t1);
        //trace("#耗时Begin-----!");
    }
    
    Timeout(const char* str)
    :_out(str)
    {
        powder::utime::gettime(&_t1);
        //trace("#耗时Begin-----!%s", str);
    }
    
    virtual ~Timeout()
    {
        struct timeval t2;
        powder::utime::gettime(&t2);
        __darwin_suseconds_t sub = t2.tv_usec - _t1.tv_usec;
        if(sub <= 0){
            trace("%s : 微秒[0] 毫秒[0] 秒执行[N]次", _out);
        }else{
            trace("%s : 微秒[%u] 毫秒[%f] 秒执行[%f]次", _out, sub, double(sub/1000),float(1000000/sub));
        }
        trace("#耗时End-----!");
    }
private:
    struct timeval _t1;
    const char* _out;
};

#endif /* time_utils_hpp */
