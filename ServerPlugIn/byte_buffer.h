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

static const int32 PACKET_BEGIN = 4;

class ByteBuffer : public ReadBytes , public WriteBytes
{
    //可以打包多个包
private:
    int32 mpos;
    
public:
    ByteBuffer()
    :mpos(0)
    {}
    
public:
    virtual void WriteBegin()
    {
        wpos(mpos + PACKET_BEGIN);
    }
    
    virtual int32 WriteEnd()
    {
        int32 mark = (int32)wpos() - mpos;
        wpos(mpos);
        append(mark - PACKET_BEGIN);
        wpos(mark + mpos);
        mpos = (int32)wpos();
        //trace("mark = %d",mark);
        //返回包长度
        return mark;
    }
    
};

#endif /* byte_buffer_h */
