//
//  error.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef error_hpp
#define error_hpp

#include "global.h"

//错误类
class Error
{
public:
    int type;
    char* msg;
public:
    Error();
    
    Error(int idx, const char* bytes);
    
    Error(const char* bytes);
    
    virtual ~Error();
    
    virtual void toString();
    
    virtual void Message(const char* bytes);
};

//
namespace error
{
    void show(const char* fm, ...);
    void show(int type, const char* fm, ...);
}

#endif /* error_hpp */
