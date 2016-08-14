//
//  packet_header.h
//  ServerPlugIn
//  协议头处理
//  Created by MikeRiy on 16/8/5.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef packet_header_h
#define packet_header_h

#include "IReader.h"

class PacketHeader : public IReader
{
public:
    int32 cmd;          //命令号
    int32 msgType;      //消息类型
    int32 version;      //服务器版本号
    int32 secretId;     //协议号(加密类型/加密规则)
    int32 viewid;       //自版本协议号(一个房间或者一个区域的id)
public:
    PacketHeader()
    :cmd(0)
    ,msgType(0)
    ,version(0)
    ,secretId(0)
    ,viewid(0)
    {
        
    }
    
    PacketHeader(ReadBytes& bytes)
    :cmd(0)
    ,msgType(0)
    ,version(0)
    ,secretId(0)
    ,viewid(0)
    {
        read(bytes);
    }
    
    PacketHeader(WriteBytes& bytes,
                 int32 cmd,
                 int32 msgType = 0,
                 int32 version = 0,
                 int32 secretid = 0,
                 int32 viewid = 0)
    :cmd(cmd)
    ,msgType(msgType)
    ,version(version)
    ,secretId(secretid)
    ,viewid(viewid)
    {
        write(bytes);
    }
    
public:
    void write(WriteBytes& bytes)override
    {
        bytes<<cmd<<msgType<<version<<secretId<<viewid;
    }
    
    void read(ReadBytes& bytes)override
    {
        bytes>>cmd>>msgType>>version>>secretId>>viewid;
    }
};


#endif /* packet_header_h */
