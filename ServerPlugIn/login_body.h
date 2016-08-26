//
//  logininfo.hpp
//  ServerPlugIn
//  登录消息
//  Created by MikeRiy on 16/8/5.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef logininfo_hpp
#define logininfo_hpp

#include "IReader.h"

class LoginBody : public IReader
{
public:
    int32 uid;
    int32 appid;
    std::string password;
    std::string macbind;
    
public:
    LoginBody()
    :uid(0)
    ,appid(0)
    {}
    
    LoginBody(ReadBytes& bytes)
    :uid(0)
    ,appid(0)
    {
        ReadFor(bytes);
    }
    
public:
    void WriteTo(WriteBytes& bytes)override
    {
        bytes<<uid<<appid<<password<<macbind;
    }
        
    void ReadFor(ReadBytes& bytes)override
    {
        bytes>>uid>>appid>>password>>macbind;
    }
        
};

#endif /* logininfo_hpp */
