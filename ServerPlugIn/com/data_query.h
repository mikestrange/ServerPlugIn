//
//  data_query.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/2.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef data_query_hpp
#define data_query_hpp

#include <stdio.h>
#include <vector>
#include "global.h"

//行的一个元素
class RowItem
{
public:
    char* key;
    char* value;
    virtual void toString();
public:
    static RowItem* create(char* key, char* value);
};



//数据库读取
class DataQuery
{
public:
    typedef std::vector<RowItem*> DBList;
public:
    DataQuery();
    
    virtual ~DataQuery();
public:
    //当前行
    virtual DBList& Next();
    //gets
public:
    virtual void row(size_t v);
    
    virtual size_t row();
    
    virtual size_t length()const;
    
    virtual bool empty()const;
    
    virtual bool exceed()const;
    
    virtual DBList* current();
    
    //当前行的一个结果(健和值)
    virtual RowItem* result(int index);
    
    //当前行的一个结果(匹配key)
    virtual RowItem* result(const char* key);
    
    //快捷方式
    RowItem* operator[] (const char* key);
    
    RowItem* operator[] (int index);
    
    //匹配当前行
    virtual bool match(const char* key, const char* value);
    
    virtual bool match(int index, const char* value);
    
    virtual void clear();
    
    //查看
    virtual void toString();
    
private:
    std::vector<DBList*> _Vector;
    size_t _row;
    
};
#endif /* data_query_hpp */
