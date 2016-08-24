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
#include "IReader.h"

class DataArray : public ReadBytes , public WriteBytes
{
public:
    DataArray(){};
    virtual ~DataArray(){};
};

#endif /* data_array_hpp */
