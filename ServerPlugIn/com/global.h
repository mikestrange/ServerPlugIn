//
//  global.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/31.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef global_h
#define global_h

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <functional>
#include <assert.h>
#include <stdarg.h>

#include "log.h"

//常用
typedef char BYTE;          //8
typedef short SHORT;        //16
typedef int DWORD;          //32
typedef long long INT64;    //64

typedef unsigned char BYTE_U;          //8 (0-255)
typedef unsigned short SHORT_U;        //16
typedef unsigned int DWORD_U;          //32
typedef unsigned long long INT64_U;    //64

//buffer
typedef char int8;
typedef unsigned char uint8;

typedef short int16;
typedef unsigned short uint16;

typedef int int32;
typedef unsigned int uint32;

typedef long long int64;

typedef unsigned long long uint64;

typedef uint64 TOKEN_T;                 //识别
typedef uint32 USER_T;                  //用户id
typedef uint16 STATUS_T;                //用户状态 255够了
typedef uint8 SEAT_T;                   //座位 255够了

#define ERROR_ASSERT(e)      assert(e)

#define SAFE_DELETE(p)      do{ if(p){ delete p; p = NULL;} }while(0)

#define SAFE_DELETE_LIST(p) do{ if(p){ delete []p; p = NULL; } }while(0)



#define MAX_INT64   max_evaluator<int64>()
#define MIN_INT64   min_evaluator<int64>()
#define MAX_UINT64  max_evaluator<uint64>()

#define MAX_INT32   max_evaluator<int32>()
#define MIN_INT32   min_evaluator<int32>()
#define MAX_UINT32  max_evaluator<uint32>()

#define MAX_INT16   max_evaluator<int16>()
#define MIN_INT16   min_evaluator<int16>()
#define MAX_UINT16  max_evaluator<uint16>()

#define MAX_INT8    max_evaluator<int8>()
#define MIN_INT8    min_evaluator<int8>()
#define MAX_UINT8   max_evaluator<uint8>()

//
#define MAX_DOUBLE  max_evaluator<double>()
#define MIN_DOUBLE  min_evaluator<double>()

#define MAX_FLOAT   max_evaluator<float>()
#define MIN_FLOAT   min_evaluator<float>()


template<typename T>
T min_evaluator()
{
    return (std::numeric_limits<T>::min)();
}

template<typename T>
T max_evaluator()
{
    return (std::numeric_limits<T>::max)();
}

//输出看台
#ifdef DEBUG
    #define trace(fm, args...)      printf(fm"\n", ##args)
#else
    #define trace(fm, args...)
#endif

//字段替换
#define STR_FORMAT(size) \
    char buf[size];\
    va_list ap;\
    va_start(ap, fm);\
    vsnprintf(buf, size, fm, ap);\
    va_end(ap);

//释放堆
#define FREE(m)  do{ if(m){ free(m); m = NULL; } }while(0)
//堆字节
#define MALLOC(size) malloc(size)


#define STATIC_CLASS(Class) \
private:\
static Class* _instance;\
public:\
static Class* getInstance();

#define STATIC_CLASS_INIT(Class)    \
Class* Class::_instance = NULL;\
Class* Class::getInstance()\
{\
    if(!_instance)\
    {\
        _instance = new Class;\
    }\
    return _instance;\
}


//namespace
#define POWDER_BEGIN namespace powder {
#define POWDER_END };

//当前文件 const char*
#define SYM_FILE    __FILE__
//当前日期
#define SYM_DATE    __DATE__
//当前时间
#define SYM_TIME    __TIME__
//当前行
#define SYM_LINE    __LINE__
//当前函数名
#define SYM_FUNC    __FUNCTION__

//替换
#define MTE_HOLDER  std::placeholders

#define block(args...) [](args)
//成员函数绑定
#define handler_0(target, func) std::bind(func, target)
#define handler_1(target, func) std::bind(func, target, MTE_HOLDER::_1)
#define handler_2(target, func) std::bind(func, target, MTE_HOLDER::_1, MTE_HOLDER::_2)
#define handler_3(target, func) std::bind(func, target, MTE_HOLDER::_1, MTE_HOLDER::_2, MTE_HOLDER::_3)
#define handler_4(target, func) std::bind(func, target, MTE_HOLDER::_1, MTE_HOLDER::_2, MTE_HOLDER::_3, MTE_HOLDER::_4)
//第一个为返回值
#define Function(R, args...) std::function<R(args)>

#endif /* global_hpp */
