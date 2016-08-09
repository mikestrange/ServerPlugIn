//
//  packet.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef packet_h
#define packet_h

#include "byte_buffer.h"
#include "time_utils.h"
#include "global.h"
#include "error.h"


#define PACKET_HEAD 4*4
#define PACKET_MAX 1024

class Packet : private ByteBuffer
{
private:
    uint32 packLen;
    size_t beginPos;        //当前指针
    time_t packTime;       //最后响应时间
public:
    Packet()
    :packLen(0)
    ,beginPos(0)
    ,packTime(0){
    
    }
    
    virtual void LoadBytes(const char* buffer, size_t len)
    {
        append(buffer, len);
        packTime = powder::stime::gettime();
    }
    
    virtual bool HasResult()
    {
        if(packLen > 0)
        {
            return wpos() >= packLen + rpos();
        }else{
            if(wpos() - rpos() >= sizeof(packLen))
            {
                packet()>>packLen;
                beginPos = rpos();
                //无字节或者字节过大
                if(packLen <= 0 || packLen > PACKET_MAX)
                {
                    error::show("this packlen is overrun %u %ld", packLen, wpos());
                }else{
                    trace("read packlen %u", packLen);
                    return wpos() >= packLen + rpos();
                }
            }
        }
        return false;
    }
    
    virtual void flush()
    {
        //消息如果读太多，溢出
        if(rpos() > wpos())
        {
            error::show("this is get overout %u %ld", rpos(), wpos());
        }
        //重置
        if(packLen > 0)
        {
            rpos(beginPos + packLen);
            beginPos = packLen = 0;
            if(rpos() == wpos()) reset();
            trace("reset flush");
        }
    }
    
    //超时心跳时间
    virtual bool OverTime() const
    {
        return powder::stime::gettime() - packTime > 60*5;
    }
    
    //获取读包
    virtual ReadBytes& packet()
    {
        return *this;
    }
};

#endif /* packet_h */
