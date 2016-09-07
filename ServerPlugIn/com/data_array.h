//
//  data_array.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/15.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef data_array_h
#define data_array_h

#include "read_bytes.h"
#include "write_bytes.h"

class ReadBytes;
class WriteBytes;
class IReader;

//快速读写
class IReader
{
public:
    virtual void WriteTo(WriteBytes& bytes)=0;
    virtual void ReadFor(ReadBytes& bytes)=0;
};

//数据流
class DataArray : public ReadBytes , public WriteBytes
{
public:
    DataArray(){};
    
    virtual ~DataArray(){};
    
public:
    void WriteObject(IReader& data)
    {
        data.WriteTo(*this);
    };
    
    void ReadObject(IReader& data)
    {
        data.ReadFor(*this);
    }
};

#endif /* data_array_hpp */
