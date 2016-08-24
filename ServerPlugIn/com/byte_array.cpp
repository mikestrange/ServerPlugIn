//
//  byte_array.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "byte_array.h"

ByteArray::ByteArray()
:_bytes(NULL)
,length(0)
{
    
}

ByteArray::ByteArray(size_t len)
:_bytes(NULL)
,length(len)
{
    _bytes = (char*)MALLOC(len);
}

ByteArray::ByteArray(const char* str, size_t len)
:_bytes(NULL)
{
    copy(str, len);
}

ByteArray::~ByteArray()
{
    clean();
}

void ByteArray::copy(const char* str, size_t len)
{
    if(length < len)
    {
        //trace("Re allocation byteArray len = %ld",len);
        clean();
        _bytes = (char*)MALLOC(len);
    }
    memcpy(_bytes, str, len);
    length = len;
}

void ByteArray::clean()
{
    FREE(_bytes);
    length = 0;
}

size_t ByteArray::size()
{
    return length;
}

char* ByteArray::bytes()
{
    return _bytes;
}