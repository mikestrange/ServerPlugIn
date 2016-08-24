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
#include "packet_header.h"

#define PACKET_BEGIN 4
#define PACKET_MAX 2048

class DataArray;
class PacketHeader;

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
        //自身头
        WriteObject(*this);
    }
    
    //自定义写入头
    virtual void WriteBegin(IReader& data)
    {
        wpos(mwpos + PACKET_BEGIN);
        WriteObject(data);
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
    
    virtual bool HasPacket()
    {
        if(packLen > 0)
        {
            if(wpos() >= packLen + rpos())
            {
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
                        return true;
                    }
                    return false;
                }
            }
        }
        return false;
    }
    
    virtual void ReadBegin()
    {
        rpos(beginPos);
        ReadObject(*this);
    }
    
    //自定义读取头
    virtual void ReadBegin(IReader& data)
    {
        rpos(beginPos);
        ReadObject(data);
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
    
    size_t wsize()
    {
        return wpos();
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
