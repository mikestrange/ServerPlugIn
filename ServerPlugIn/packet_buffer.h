//
//  byte_buffer.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef byte_buffer_h
#define byte_buffer_h

#include "data_array.h"

#define PACKET_BEGIN 4
#define PACKET_MAX 2048

class DataArray;

//协议头
class PacketHeader
{
public:
    int32 cmd;          //命令号
    int32 msgType;      //消息类型(服务器类型)
    int32 version;      //服务器版本号                (客户端写入)
    int32 secretId;     //协议号(加密类型/加密规则)     (客户端写入)
    int32 viewid;       //自版本协议号(一个房间或者一个区域的id)
    
public:
    PacketHeader()
    :cmd(0)
    ,msgType(0)
    ,version(0)
    ,secretId(0)
    ,viewid(0){}
    
    virtual ~PacketHeader(){}
    
    void setHeader(int32 _cmd, int32 _msg = 0)
    {
        cmd = _cmd;
        if(_msg > 0) msgType = _msg;
    }
    
    void CopyHeader(PacketHeader& data)
    {
        cmd = data.cmd;
        msgType = data.msgType;
        version = data.version;
        secretId = data.secretId;
        viewid = data.viewid;
    }
    
protected:
    void OutputHeader(DataArray& buf)
    {
        buf>>cmd>>msgType>>version>>secretId>>viewid;
    }
    
    void InputHeader(DataArray& buf)
    {
        buf<<cmd<<msgType<<version<<secretId<<viewid;
    }
    
};

//协议处理
class PacketBuffer : public DataArray , public PacketHeader
{
private:
    int32 mwpos;
    
private:
    size_t beginPos;
    uint32 packLen;
    
public:
    PacketBuffer()
    :mwpos(0)
    ,beginPos(0)
    ,packLen(0)
    {}
    
public:
    virtual void WriteBegin()
    {
        wpos(mwpos + PACKET_BEGIN);
        //写入头
        InputHeader(self());
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
    
    
    //开始读
    virtual void LoadBytes(const char* bytes, size_t len)
    {
        append(bytes, len);
    }
    
    virtual bool ReadBegin()
    {
        if(packLen > 0)
        {
            if(wpos() >= packLen + rpos())
            {
                OutputHeader(self());
                return true;
            }
            return false;
        }else{
            if(wpos() - rpos() >= sizeof(packLen))
            {
                self()>>packLen;
                beginPos = rpos();
                //无字节或者字节过大
                if(packLen <= 0 || packLen > PACKET_MAX)
                {
                    error::show("this packlen is overrun %u %ld", packLen, wpos());
                }else{
                    trace(">>##ReadBegin Pocket Length %u %zu %zu", packLen, rpos(), wpos());
                    if(wpos() >= packLen + rpos())
                    {
                        OutputHeader(self());
                        return true;
                    }
                    return false;
                }
            }
        }
        return false;
    }
    
    virtual void ReadEnd()
    {
        //消息如果读太多，溢出
        if(rpos() > wpos())
        {
            error::show("this is get overout %u %ld", rpos(), wpos());
        }
        //读完无子节重置
        if(packLen > 0)
        {
            //设置到末尾
            rpos(beginPos + packLen);
            beginPos = packLen = 0;
            if(rpos() == wpos()) reset();
            trace(">>##ReadEnd is Flush");
        }
    }
    
    
    //当前包剩余可读子节
    virtual size_t subLeng()
    {
        return packLen - (_rpos - beginPos);
    }
    
    PacketBuffer& self()
    {
        return *this;
    }
    
    ReadBytes& Read()
    {
        return *this;
    }
    
    WriteBytes& Write()
    {
        return *this;
    }
};

#endif /* byte_buffer_h */
