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

//头大小
#define PACKET_BEGIN 4
//长度
#define PACKET_MAX 2048

class DataArray;
class PacketHeader;

class PacketBuffer : public DataArray , public PacketHeader
{
private:
    int32 m_wpos;
private:
    size_t m_rpos;
private:
    uint32 packet_size;
    
public:
    PacketBuffer()
    :m_wpos(0)
    ,m_rpos(0)
    ,packet_size(0)
    {}
    
public:
    virtual void WriteBegin()
    {
        wpos(m_wpos + PACKET_BEGIN);
        WriteObject(*this);
    }
    
    //自定义
    virtual void WriteBegin(IReader& data)
    {
        wpos(m_wpos + PACKET_BEGIN);
        WriteObject(data);
    }
    
    virtual int32 WriteEnd()
    {
        int32 mark = (int32)wpos() - m_wpos;
        wpos(m_wpos);
        append(mark - PACKET_BEGIN);
        wpos(mark + m_wpos);
        m_wpos = (int32)wpos();
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
        if(packet_size > 0)
        {
            if(wpos() >= packet_size + rpos())
            {
                return true;
            }
            return false;
        }else{
            if(wpos() - rpos() >= sizeof(packet_size))
            {
                self()>>packet_size;
                m_rpos = rpos();
                if(packet_size <= 0 || packet_size > PACKET_MAX)
                {
                    error::show("this packlen is overrun %u %ld", packet_size, wpos());
                }else{
                    trace(">>##ReadBegin Pocket Length %u %zu %zu", packet_size, rpos(), wpos());
                    if(wpos() >= packet_size + rpos())
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
        rpos(m_rpos);
        ReadObject(*this);
    }
    
    //自定义读取头
    virtual void ReadBegin(IReader& data)
    {
        rpos(m_rpos);
        ReadObject(data);
    }
    
    virtual void ReadEnd()
    {
        //max overflow
        if(rpos() > wpos())
        {
            error::show("this is get overout %u %ld", rpos(), wpos());
        }
        //reset
        if(packet_size > 0)
        {
            rpos(m_rpos + packet_size);
            packet_size = m_rpos = 0;
            if(rpos() == wpos()) reset();
            trace(">>##ReadEnd is Flush");
        }
    }
    
    size_t wsize()
    {
        return wpos();
    }
    
    //Current package remaining readable sub section
    virtual size_t subLeng()
    {
        return packet_size - (_rpos - m_rpos);
    }
    
    PacketBuffer& self()
    {
        return *this;
    }
};

#endif /* byte_buffer_h */
