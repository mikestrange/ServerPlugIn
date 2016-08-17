//
//  math_utils.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/16.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "math_utils.h"

static bool is_set = false;

POWDER_BEGIN


void SRandom()
{
    is_set = true;
    srand((unsigned)time(NULL));
}

int Random(int f)
{
    if(!is_set) SRandom();
    return rand()%f;
}

int Random(int left, int right)
{
    if(right > left)
    {
        return left + Random(right - left);
    }
    return right + Random(left - right);
}

POWDER_END


