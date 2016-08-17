//
//  IReader.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef IReader_h
#define IReader_h

#include <stdio.h>
#include "packet_buffer.h"

class WriteBytes;
class ReadBytes;

class IReader
{
public:
    virtual void write(WriteBytes& bytes)=0;
    virtual void read(ReadBytes& bytes)=0;
};

#endif /* IReader_h */
