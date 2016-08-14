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
    //必须
    int32 uid;
    int32 appid;
    std::string password;
    std::string macbind;
    //可选(返回时需要)
    std::string name;
    int money;
    int exp;
    
public:
    LoginBody()
    :uid(0)
    ,appid(0)
    ,money(0)
    ,exp(0)
    {}
    
    LoginBody(ReadBytes& bytes)
    :uid(0)
    ,appid(0)
    ,money(0)
    ,exp(0)
    {
        read(bytes);
    }
    
public:
    void write(WriteBytes& bytes)override
    {
        bytes<<uid<<appid<<password<<macbind;
    }
        
    void read(ReadBytes& bytes)override
    {
        bytes>>uid>>appid>>password>>macbind;
    }
        
};

#endif /* logininfo_hpp */
