//
//  buffer_base.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/3.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef buffer_base_h
#define buffer_base_h

#include <vector>
#include <string.h>
#include "error.h"
#include "global.h"

class BufferInterface
{
protected:
    size_t _rpos, _wpos;
    std::vector<uint8> _storage;
    
public:
    BufferInterface()
    : _rpos(0)
    , _wpos(0)
    {
        _storage.reserve(64);
    }
    
protected:
    const uint8* contents()const
    {
        return &_storage[0];
    }
    
    size_t size() const
    {
        return _storage.size();
    }
    
    bool empty() const
    {
        return _storage.empty();
    }
    
    //appends
public:
    BufferInterface& append(const std::string& str)
    {
        return append((uint8 const*)str.c_str(), str.size() + 1);
    }
    
    BufferInterface& append(const char* src, size_t cnt)
    {
        return append((const uint8 *)src, cnt);
    }
    
    template<class T> BufferInterface& append(const T* src, size_t cnt)
    {
        return append((const uint8 *)src, cnt * sizeof(T));
    }
    
    BufferInterface& append(const uint8* src, size_t cnt)
    {
        if (_storage.size() < _wpos + cnt)
        {
            _storage.resize(_wpos + cnt);
        }
        memcpy(&_storage[_wpos], src, cnt);
        _wpos += cnt;
        return *this;
    }
    
    BufferInterface& append(BufferInterface& buffer, size_t pos, size_t cnt)
    {
        if (_storage.size() < _wpos + cnt)
        {
            _storage.resize(_wpos + cnt);
        }
        memcpy(&_storage[_wpos], &buffer[pos], cnt);
        _wpos += cnt;
        return *this;
    }
    
    template <typename T> BufferInterface& append(T value)
    {
        return append((uint8*)&value, sizeof(value));
    }
    
    void put(size_t pos, const uint8 *src, size_t cnt)
    {
        if(pos + cnt > size())
        {
            error::show("put error: %zu %ld", pos, cnt);
        }
        memcpy(&_storage[pos], src, cnt);
    }
    
    template <typename T> void put(size_t pos, T value)
    {
        put(pos, (uint8 *)&value, sizeof(value));
    }
    
    //reads
public:
    template <typename T> T read()
    {
        T r = read<T>(_rpos);
        _rpos += sizeof(T);
        return r;
    }
    
    template <typename T> T read(size_t pos) const
    {
        if(pos + sizeof(T) > size())
        {
            error::show("read T error: %ld %ld %ld", pos, sizeof(T), size());
        }
        T val = *((T const*)&_storage[pos]);
        return val;
    }
    
    BufferInterface& read(char* dest, size_t len)
    {
        if(_rpos  + len > size())
        {
            error::show("read uint8 error: %ld %ld %ld", _rpos, len, size());
        }
        memcpy(dest, &_storage[_rpos], len);
        _rpos += len;
        return *this;
    }
    
public:
    virtual uint8& operator[](size_t const pos)
    {
        if (pos >= size())
        {
            error::show("get error 1: %ld %ld",pos, size());
        }
        return _storage[pos];
    }
    
    virtual uint8 const& operator[](size_t const pos) const
    {
        if (pos >= size())
        {
            error::show("get error 2: %ld %ld",pos, size());
        }
        return _storage[pos];
    }
    
    //get
    virtual uint8& get(size_t const pos)
    {
        if (pos >= size())
        {
            error::show("get error 1: %ld %ld",pos, size());
        }
        return _storage[pos];
    }
    
    virtual void clear()
    {
        _rpos = _wpos = 0;
        _storage.clear();
    }
    
    virtual void reset()
    {
        _rpos = _wpos = 0;
    }
    
    //0-newsize合法，之外不合法（直接构造了新元素）
    virtual void resize(size_t newsize)
    {
        if (newsize > size())
        {
            _storage.resize(newsize);
        }
    }
    
    virtual size_t size()
    {
        return _storage.size();
    }
    
    // 新元素还没有构造, 此时不能用[]访问元素
    virtual void reserve(size_t ressize)
    {
        if (ressize > size())
        {
            _storage.reserve(ressize);
        }
    }
};

#endif /* buffer_base_h */
