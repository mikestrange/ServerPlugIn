//
//  data_bank.h
//  ServerPlugIn
//  1,/usr/local/mysql/include
//  2,/usr/local/mysql/lib
//  3,libmysqld.a libmysqlclient.a
//  4,-lrt -lz -lm
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef data_bank_h
#define data_bank_h

//定义数据库
//#define MY_SQL

#include <stdio.h>
#include <vector>
#include <map>

#ifdef MY_SQL
#include <mysql.h>
#endif

#include "global.h"
#include "data_query.h"

//数据库服务器
class DataBank
{
public:
    DataBank();
    
    virtual ~DataBank();
    
public:
    virtual bool connent(const char* host,
                         const char* user,
                         const char* pswd,
                         const char* table,
                         unsigned int port);
    
    virtual void close();
    
    virtual bool isConnected()const;
    
    virtual bool apply(const char* sql);
    
    virtual bool applyFormat(const char* fm, ...);
    
    virtual bool find(DataQuery& query, const char* sql);
    
    virtual bool findFormat(DataQuery& query, const char* fm,...);
    
public:
    //test
    virtual void test_connent()
    {
        const char *host = "localhost";
        const char *user = "root";
        const char *pswd = "abc";
        const char *table = "user_info";
        unsigned int port = 3306;
        connent(host, user, pswd, table, port);
        //find("select * from user_info.player");
    }
protected:
    bool isconnected = false;
#ifdef MY_SQL
private:
    MYSQL myCont;
#endif
};


#endif /* data_bank_h */
