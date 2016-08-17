//
//  string_utils.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef string_utils_h
#define string_utils_h

#include "global.h"

namespace StringUtil
{
    std::string format(const char* fm, ...);
    
    
    bool equal(const char* src, const char* other);
    
    bool equal(std::string& src, const char* other);
    
    //字符范围内
    bool scope(std::string &str,int left, int right);
}

namespace Basal
{
    int parseInt(const char* src);
    int parseInt(std::string& src);
}



#endif /* string_utils_hpp */
