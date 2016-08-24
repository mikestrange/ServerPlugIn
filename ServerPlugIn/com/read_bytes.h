//
//  read_bytes.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef read_bytes_h
#define read_bytes_h

#include "buffer_base.h"
#include "IReader.h"

class IReader;

class ReadBytes : virtual public BufferInterface
{
public:
    ReadBytes(){
        
    }
public:
    //read
    ReadBytes &operator>>(bool &value)
    {
        value = read<char>()> 0 ? true : false;
        return *this;
    }
    
    ReadBytes &operator>>(uint8 &value)
    {
        value = read<uint8>();
        return *this;
    }
    
    ReadBytes &operator>>(uint16 &value)
    {
        value = read<uint16>();
        return *this;
    }
    
    ReadBytes &operator>>(uint32 &value)
    {
        value = read<uint32>();
        return *this;
    }
    
    ReadBytes &operator>>(uint64 &value)
    {
        value = read<uint64>();
        return *this;
    }
    
    ReadBytes &operator>>(int8 &value)
    {
        value = read<int8>();
        return *this;
    }
    
    ReadBytes &operator>>(int16 &value)
    {
        value = read<int16>();
        return *this;
    }
    
    ReadBytes &operator>>(int32 &value)
    {
        value = read<int32>();
        return *this;
    }
    
    ReadBytes &operator>>(int64 &value)
    {
        value = read<int64>();
        return *this;
    }
    
    ReadBytes &operator>>(float &value)
    {
        value = read<float>();
        return *this;
    }
    
    ReadBytes &operator>>(double &value)
    {
        value = read<double>();
        return *this;
    }
    
    ReadBytes &operator>>(std::string& str)
    {
        str.clear();
        int32 len = read<int32>();
        //
        for(int i = 0; i < len; i++)
        {
            str += read<char>();
        }
        return *this;
    }
    
    void ReadBuffer(BufferInterface& buffer, size_t len)
    {
        if(len > 0){
            buffer.append(&get(_rpos), len);
        }
    }
    
    void ReadObject(IReader& target);
    
    //r pos
    virtual size_t rpos()const
    {
        return _rpos;
    }
    
    virtual size_t rpos(size_t rpos_)
    {
        _rpos = rpos_;
        return _rpos;
    }
};



#endif /* read_bytes_h */
