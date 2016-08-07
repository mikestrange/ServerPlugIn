//
//  byte_array.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef byte_array_h
#define byte_array_h

#include <stdio.h>
#include "global.h"

class ByteArray
{
private:
    char* _bytes;
    size_t length;
public:
    ByteArray();
    
    ByteArray(size_t len);
    
    ByteArray(const char* str, size_t len);
    
    virtual ~ByteArray();
    
public:
    void copy(const char* str, size_t len);
    
    void clean();
    
    size_t size();
    
    char* bytes();
};

#endif /* byte_array_h */
