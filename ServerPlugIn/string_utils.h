//
//  string_utils.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef string_utils_h
#define string_utils_h

#include <stdio.h>
#include "global.h"

namespace StringUtil
{
    std::string format(const char* fm, ...);
    
    bool scope(std::string &str,int left, int right);
}

#endif /* string_utils_hpp */
