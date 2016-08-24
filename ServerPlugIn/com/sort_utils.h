//
//  sort_utils.hpp
//  ServerPlugIn
//  排序相关
//  Created by MikeRiy on 16/8/15.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef sort_utils_h
#define sort_utils_h

#include "global.h"

namespace utils
{
    //排序(小到大)
    template<class T, class F> void SortOn(T* ls, int size, F _f)
    {
        for(int i = 0; i < size; i++)
        {
            for(int j = size - 1; j > i; j--)
            {
                if(_f(ls[i], ls[j]) > 0)
                {
                    T temp = ls[i];
                    ls[i] = ls[j];
                    ls[j] = temp;
                }
            }
        }
    }
    
    //拷贝
    template<class T> void Copy(T* ctr, T* src, int size)
    {
        for(int i = 0; i < size; i++)
        {
            ctr[i] = src[i];
        }
    }
}



#endif /* sort_utils_hpp */
