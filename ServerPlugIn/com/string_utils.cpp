//
//  string_utils.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "string_utils.h"


namespace StringUtil
{
    std::string format(const char* fm, ...)
    {
        std::string ret;
        STR_FORMAT(1024);
        ret = buf;
        return ret;
    }
    
    bool equal(const char* src, const char* other)
    {
        return strcmp(src, other) == 0;
    }
    
    bool equal(std::string& src, const char* other)
    {
        return equal(src.c_str(), other);
    }
    
    bool scope(std::string &str, int left, int right)
    {
        return str.length()>=left && str.length() <= right;
    }
}


namespace Basal
{    
    int parseInt(const char* src)
    {
        return atof(src);
    }
    
    int parseInt(std::string& src)
    {
        return parseInt(src.c_str());
    }
}

