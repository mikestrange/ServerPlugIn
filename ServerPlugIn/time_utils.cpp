//
//  time_utils.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "time_utils.h"

static const time_t RUN_TIME = time(NULL);

static const clock_t START_TIME = clock();

//精确纳秒
//static double orwl_timebase = 0.0;
//static uint64_t orwl_timestart = 0;
static __darwin_suseconds_t orwl_space = 1000;


POWDER_BEGIN

    namespace stime
    {
        time_t gettime()
        {
            return time(NULL);
        }
        
        time_t overtime(time_t t)
        {
            return time(NULL) - t;
        }
        
        time_t runtime()
        {
            return time(NULL) - RUN_TIME;
        }
    }

    //
    namespace mtime
    {
        clock_t gettime()
        {
            return clock();
        }
        
        double runtime()
        {
            return ((double)(clock() - START_TIME))/CLOCKS_PER_SEC;
        }
    }

    namespace utime
    {
        struct timeval* gettime(struct timeval *t)
        {
            gettimeofday(t, 0);
            return t;
        }
        
        struct timeval* addtime(struct timeval *t,__darwin_time_t sec,__darwin_suseconds_t usec)
        {
            __darwin_suseconds_t total = t->tv_usec + usec;
            if(total >= UVAL_TIME){
                t->tv_sec = t->tv_sec + sec + 1;
                t->tv_usec = total - UVAL_TIME;
            }else{
                t->tv_sec = t->tv_sec + sec;
                t->tv_usec = total;
            }
            return t;
        }
        
        int subtime(struct timeval* result, struct timeval* sub, struct timeval* head)
        {
            if (sub->tv_sec > head->tv_sec) return -1;
            if (sub->tv_sec == head->tv_sec && sub->tv_usec > head->tv_usec) return -1;
            //
            result->tv_sec = head->tv_sec - sub->tv_sec;
            result->tv_usec = head->tv_usec - sub->tv_usec;
            if(result->tv_usec < 0)
            {
                result->tv_sec--;
                result->tv_usec += UVAL_TIME;
            }
            return 0;
        }
        //end
    }


    namespace ntime
    {
        struct timespec* gettime(struct timespec *t)
        {
            struct timeval now;
            gettimeofday(&now, NULL);
            t->tv_sec = now.tv_sec;
            t->tv_nsec = now.tv_usec * orwl_space;
            return t;
        }
        
        struct timespec* addtime(struct timespec *t,__darwin_time_t sec,long nsec)
        {
            long total = t->tv_nsec + nsec;
            if(total >= NVAL_TIME){
                t->tv_sec = t->tv_sec + sec + 1;
                t->tv_nsec = total - NVAL_TIME;
            }else{
                t->tv_sec = t->tv_sec + sec;
                t->tv_nsec = total;
            }
            return t;
        }
        
        //-1表示sub超过了head
        int subtime(struct timespec* result, struct timespec* sub, struct timespec* head)
        {
            if (sub->tv_sec > head->tv_sec) return -1;
            if (sub->tv_sec == head->tv_sec && sub->tv_nsec > head->tv_nsec) return -1;
            //
            result->tv_sec = head->tv_sec - sub->tv_sec;
            result->tv_nsec = head->tv_nsec - sub->tv_nsec;
            if(result->tv_nsec < 0)
            {
                result->tv_sec--;
                result->tv_nsec+=NVAL_TIME;
            }
            return 0;
        }
    }

POWDER_END


