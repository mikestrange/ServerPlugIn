//
//  byte_buffer.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef byte_buffer_h
#define byte_buffer_h

#include <stdio.h>
#include <vector>
#include <string.h>

#include "read_bytes.h"
#include "write_bytes.h"
#include "IReader.h"

#define PACKET_BEGIN 4

class ByteBuffer : public ReadBytes , public WriteBytes
{
    //可以打包多个包
private:
    int32 mwpos;
    int32 mrpos;
    
public:
    ByteBuffer()
    :mwpos(0)
    ,mrpos(0)
    {}
    
public:
    virtual void WriteBegin()
    {
        wpos(mwpos + PACKET_BEGIN);
    }
    
    virtual int32 WriteEnd()
    {
        int32 mark = (int32)wpos() - mwpos;
        wpos(mwpos);
        append(mark - PACKET_BEGIN);
        wpos(mark + mwpos);
        mwpos = (int32)wpos();
        //trace("mark = %d",mark);
        //返回包长度
        return mark;
    }
    
    virtual void ReadBegin()
    {
        
    }
    
    virtual void ReadEnd()
    {
        
    }
    
    ByteBuffer& self()
    {
        return *this;
    }
};

#endif /* byte_buffer_h */
