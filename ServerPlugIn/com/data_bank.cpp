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

#include "data_bank.h"

DataBank::DataBank()
:isconnected(false)
{

}
    
DataBank::~DataBank()
{
    close();
}
    
bool DataBank::connent(const char* host,
                       const char* user,
                       const char* pswd,
                       const char* table,
                       unsigned int port)
{
    if(isConnected())
    {
        trace("mysql is connect");
        return true;
    }
    isconnected = false;
    #ifdef MY_SQL
    //初始化
    if(mysql_init(&myCont)==NULL)
    {
        Log::error("mysql init error");
        return false;
    };
    //连接数据库
    if(mysql_real_connect(&myCont, host, user, pswd, table, port, NULL, 0))
    {
        isconnected = true;
        //设置编码格式 写入
        mysql_set_character_set(&myCont, "utf8");
        Log::info("mysql connect succeed!");
    }else{
        mysql_close(&myCont);
        Log::error("mysql failed!");
    };
    #else
        Log::warn("undefined MY_SQL");
    #endif
    return isconnected;
}

void DataBank::close()
{
    if(isconnected)
    {
        isconnected = false;
        #ifdef MY_SQL
        mysql_close(&myCont);
        #endif
        Log::info("mysql close ok!");
    }else{
        trace("mysql close error: is no open");
    }
}

bool DataBank::isConnected()const
{
    return isconnected;
}

bool DataBank::apply(const char* sql)
{
    if(!isConnected()){
        Log::warn("mysql apply error: is no open");
        return false;
    }
    trace("sql execute:%s",sql);
    int ret = -1;
    #ifdef MY_SQL
    ret = mysql_query(&myCont, sql);
    #endif
    if(ret != 0)
    {
        Log::warn("mysql apply error:%d", ret);
    }
    return ret == 0;
}

bool DataBank::applyFormat(const char* fm, ...)
{
    static const int SQL_SIZE = 1024;
    STR_FORMAT(SQL_SIZE);
    return apply(buf);
}

bool DataBank::findFormat(DataQuery& query, const char* fm,...)
{
    static const int SQL_SIZE = 255;
    STR_FORMAT(SQL_SIZE);
    return find(query, buf);
}

bool DataBank::find(DataQuery& query, const char* sql)
{
    //"select * from player"
    #ifdef MY_SQL
    if(apply(sql))
    {
        //保存查询到的数据到result
        MYSQL_RES *result = mysql_store_result(&myCont);
        if(result)
        {
            MYSQL_FIELD *fd;
            std::vector<MYSQL_FIELD*> keys;
            for(int i = 0; (fd = mysql_fetch_field(result)); i++)
            {
                keys.push_back(fd);//fd->name
            }
            int line = mysql_num_fields(result);
            MYSQL_ROW sql_row;
            while((sql_row = mysql_fetch_row(result)))
            {
                DataQuery::DBList &row = query.Next();
                //trace("------line begin------");
                for(int i = 0; i < line; i++)
                {
                    MYSQL_FIELD* field = keys[i];
                    //当前行添加
                    row.push_back(RowItem::create(field->name, sql_row[i]));
                    //hash[field->name] = sql_row[i];
                    //trace("key = %s, value = %s", field->name, hash[field->name]);
                }
                //trace("------line end------");
            }
            //释放结果资源
            mysql_free_result(result);
        }else{
            Log::warn("mysql find result error");
        }
    }
    #endif
    return false;
}
