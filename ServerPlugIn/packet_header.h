//
//  world_header.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/23.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef world_header_hpp
#define world_header_hpp

#include "global.h"
#include "IReader.h"

class PacketHeader : public IReader
{
public:
    int32 cmd;          //命令号
    int32 type;         //消息类型(服务器类型)
    int32 version;      //服务器版本号(可以不用管)
    int32 view_id;       //自版本协议号(一个房间或者一个区域的id)
    TOKEN_T token_id;    //用户识别标志(作为会话id)
    
public:
    PacketHeader()
    :cmd(0)
    ,type(0)
    ,version(0)
    ,view_id(0)
    ,token_id(0){}

    virtual ~PacketHeader(){}

    void setHeader(int32 _cmd, int32 _type = 0, TOKEN_T tid = 0, int32 vid = 0, int32 ver = 0)
    {
        cmd = _cmd;
        type = _type;
        version = ver;
        view_id = vid;
        token_id = tid;
    }


public:
    virtual void WriteTo(WriteBytes& bytes)override
    {
        bytes<<cmd<<type<<version<<view_id<<token_id;
    }
    
    virtual void ReadFor(ReadBytes& bytes)override
    {
        bytes>>cmd>>type>>version>>view_id>>token_id;
    }
};


#endif /* world_header_hpp */
