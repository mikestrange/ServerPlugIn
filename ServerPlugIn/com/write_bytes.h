//
//  write_bytes.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef write_bytes_h
#define write_bytes_h

#include "buffer_base.h"

class WriteBytes : virtual public BufferInterface
{
public:
    WriteBytes(){
        
    }
public:
    //write
    WriteBytes &operator<<(uint8 value)
    {
        append<uint8>(value);
        return *this;
    }
    
    WriteBytes &operator<<(uint16 value)
    {
        append<uint16>(value);
        return *this;
    }
    
    WriteBytes &operator<<(uint32 value)
    {
        append<uint32>(value);
        return *this;
    }
    
    WriteBytes &operator<<(uint64 value)
    {
        append<uint64>(value);
        return *this;
    }
    
    WriteBytes &operator<<(int8 value)
    {
        append<int8>(value);
        return *this;
    }
    
    WriteBytes &operator<<(int16 value)
    {
        append<int16>(value);
        return *this;
    }
    
    WriteBytes &operator<<(int32 value)
    {
        append<int32>(value);
        return *this;
    }
    
    WriteBytes &operator<<(int64 value)
    {
        append<int64>(value);
        return *this;
    }
    
    // floating points
    WriteBytes &operator<<(float value)
    {
        append<float>(value);
        return *this;
    }
    
    WriteBytes &operator<<(double value)
    {
        append<double>(value);
        return *this;
    }
    
    WriteBytes &operator<<(const char *str)
    {
        int32 len = (int32)strlen(str);
        append<int32>(len);
        if(len > 0){
            append(str, len);
        }
        return *this;
    }
    
    WriteBytes &operator<<(const std::string &value)
    {
        int32 len = (int32)value.length();
        append<int32>(len);
        if(len > 0){
            append((const char*)value.c_str(), len);
        }
        return *this;
    }
    
    void WriteChars(const char* bytes, size_t len)
    {
        append<int32>((int32)len);
        if(len > 0)
        {
            append(bytes, len);
        }
    }
    
    //从buffer写入
    void WriteBuffer(BufferInterface& buffer, int pos, size_t len)
    {
        if(len > 0)
        {
            append(&buffer[pos], len);
        }
    }
    
    //w pos
    virtual size_t wpos()const
    {
        return _wpos;
    }
    
    virtual size_t wpos(size_t wpos_)
    {
        _wpos = wpos_;
        return _wpos;
    }
};

#endif /* write_bytes_h */
