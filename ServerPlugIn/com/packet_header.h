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
#include "data_array.h"

class PacketHeader : public IReader
{
private:
    int32 cmd;              //命令号
    int32 type;             //消息类型(服务器类型)
    int32 version;          //服务器版本号(可以不用管)
    int32 view_id;          //自版本协议号(一个房间或者一个区域的id)
    TOKEN_T token_id;       //用户识别标志(作为会话id)
    
public:
    PacketHeader()
    :cmd(0)
    ,type(0)
    ,version(0)
    ,view_id(0)
    ,token_id(0){}

    virtual ~PacketHeader(){}

    virtual void setBegin(int32 $cmd, int32 $type = 0, TOKEN_T $tid = 0, int32 $vid = 0, int32 $ver = 0)
    {
        cmd = $cmd;
        type = $type;
        version = $ver;
        view_id = $vid;
        token_id = $tid;
    }
    
    virtual void CopyBegin(PacketHeader& data)
    {
        cmd = data.cmd;
        type = data.type;
        version = data.version;
        view_id = data.view_id;
        token_id = data.token_id;
    }
    
public:
    int32 getCmd()
    {
        return cmd;
    }
    
    int32 getType()
    {
        return type;
    }
    
    int32 getViewId()
    {
        return view_id;
    }
    
    int32 getVersion()
    {
        return version;
    }
    
    TOKEN_T getTokenId()
    {
        return token_id;
    }

public:
    virtual void WriteTo(WriteBytes& bytes)override
    {
        bytes<<cmd<<type<<version<<view_id<<token_id;
        //Log::debug("write %d %d %d %d", cmd, type, version, view_id);
    }
    
    virtual void ReadFor(ReadBytes& bytes)override
    {
        bytes>>cmd>>type>>version>>view_id>>token_id;
        //Log::debug("read %d %d %d %d", cmd, type, version, view_id);
    }
};


#endif /* world_header_hpp */
