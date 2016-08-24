//
//  hash_map.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/15.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef hash_map_h
#define hash_map_h

#include <map>
#include "error.h"
#include "global.h"

template<class K,class V>
class HashMap
{
public:
    typedef typename std::map<K, V>::iterator Iterator;
public:
    HashMap(){}
    
    HashMap(HashMap<K,V>& target)
    {
        copy(target);
    }
    
    virtual ~HashMap()
    {
        
    }
    
public:
    //超过上限
    virtual bool IsUpper()
    {
        return tab.size() >= tab.max_size();
    }
    
    virtual V find(K k)
    {
        Iterator iter = tab.find(k);
        if(iter != tab.end())
        {
            return iter->second;
        }
        return 0;
    }
    
    bool has(K k)
    {
        return tab.find(k) != tab.end();
    }
    
    virtual V put(K k, V v)
    {
        if(has(k))
        {
            error::show("error hashmap:put have key:%d", k);
        }
        tab.insert(std::pair<K,V>(k, v));
        return v;
    }
    
    size_t size()
    {
        return tab.size();
    }
    
    Iterator begin()
    {
        return tab.begin();
    }
    
    Iterator end()
    {
        return tab.end();
    }
    
    void remove(Iterator iter)
    {
        tab.erase(iter);
    }
    
    virtual V remove(K k)
    {
        Iterator iter = tab.find(k);
        if(iter != tab.end())
        {
            V v = iter->second;
            tab.erase(iter);
            return v;
        }
        return 0;
    }
    
    virtual void copy(HashMap<K,V>& src)
    {
        Iterator iter;
        for(iter = src.begin(); iter!=src.end(); ++iter)
        {
            put(iter->first, iter->second);
        }
    }
    
    virtual void clear()
    {
        tab.clear();
    }
    
    template<class Block>
    void clear(Block func)
    {
        Iterator iter;
        for(iter = tab.begin(); iter!=tab.end(); ++iter)
        {
            func(iter->second);
        }
        tab.clear();
    }
    
    //遍历(k,v)
    template<class Block>
    void eachMaps(Block func)
    {
        Iterator iter;
        for(iter = tab.begin(); iter != tab.end(); ++iter)
        {
            func(iter->first, iter->second);
        }
    }
    
public:
    std::map<K,V>& proto()
    {
        return tab;
    }
    
private:
    std::map<K,V> tab;
};

#endif /* hash_map_hpp */
