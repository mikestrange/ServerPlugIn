//
//  packet_header.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/5.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef packet_header_h
#define packet_header_h

#include <stdio.h>

#include "IReader.h"

class HeadInfo : public IReader
{
public:
    int32 cmd;      //命令号
    int32 msgType;  //消息类型
    int32 version;  //服务器版本号
    int32 file;     //协议号
public:
    HeadInfo()
    :cmd(0)
    ,msgType(0)
    ,version(0)
    ,file(0)
    {
        
    }
    
    HeadInfo(ReadBytes& bytes)
    :cmd(0)
    ,msgType(0)
    ,version(0)
    ,file(0)
    {
        read(bytes);
    }
    
    HeadInfo(WriteBytes& bytes, int32 cmd, int32 msgType = 0, int32 version = 0, int32 file = 0)
    :cmd(cmd)
    ,msgType(msgType)
    ,version(version)
    ,file(file)
    {
        write(bytes);
    }
    
public:
    void write(WriteBytes& bytes)override
    {
        bytes<<cmd<<msgType<<version<<file;
    }
    
    void read(ReadBytes& bytes)override
    {
        bytes>>cmd>>msgType>>version>>file;
    }
};


#endif /* packet_header_hpp */
