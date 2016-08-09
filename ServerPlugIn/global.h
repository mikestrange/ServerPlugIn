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


#define ERROR_ASSERT(e)      assert(e)

#define SAFE_DELETE(p)      do{ if(p){ delete p; p = NULL;} }while(0)

#define SAFE_DELETE_LIST(p) do{ if(p){ delete []p; p = NULL; } }while(0)

//输出看台
#define trace(fm, args...)      printf(fm"\n", ##args)

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
