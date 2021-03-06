//
//  regdata.hpp
//  ServerPlugIn
//  注册消息
//  Created by MikeRiy on 16/8/5.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef reginfo_hpp
#define reginfo_hpp

#include "data_array.h"

class RegBody : public IReader
{
public:
    int32 uid;
    int32 appid;
    std::string password;
    std::string macbind;
    std::string name;
public:
    RegBody()
    :uid(0)
    ,appid(0)
    {
    
    }
    
    RegBody(ReadBytes& bytes)
    :uid(0)
    ,appid(0)
    {
        ReadFor(bytes);
    }
    
public:
    void WriteTo(WriteBytes& bytes)override
    {
        bytes<<uid<<appid<<password<<macbind<<name;
    }
        
    void ReadFor(ReadBytes& bytes)override
    {
        bytes>>uid>>appid>>password>>macbind>>name;
    }
};
        


#endif /* reginfo_hpp */
