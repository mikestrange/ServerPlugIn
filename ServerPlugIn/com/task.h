//
//  task_base.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef task_base_h
#define task_base_h

//#include <stdio.h>
//args为内存(必须为静态回调)
//typedef std::function<void(int, void*)> TASK_CALL;

class Task
{
public:
    Task(){};
    
    virtual ~Task(){};
    
    virtual void perform(){}
    
    virtual void destroy(){}
};

namespace Block
{
    template<class Class, typename ParamType1 = void, typename ParamType2 = void, typename ParamType3 = void, typename ParamType4 = void>
    class _Callback
    {
    protected:
        
        typedef void (Class::*Method)(ParamType1, ParamType2, ParamType3, ParamType4);
        Class* m_object;
        Method m_method;
        ParamType1 m_param1;
        ParamType2 m_param2;
        ParamType3 m_param3;
        ParamType4 m_param4;
        void _Execute() { (m_object->*m_method)(m_param1, m_param2, m_param3, m_param4); }
        
    public:
        _Callback(Class* object, Method method, ParamType1 param1, ParamType2 param2, ParamType3 param3, ParamType4 param4)
        : m_object(object), m_method(method),
        m_param1(param1), m_param2(param2), m_param3(param3), m_param4(param4)
        {
        }
        
        _Callback(_Callback<Class, ParamType1, ParamType2, ParamType3, ParamType4> const& cb)
        : m_object(cb.m_object), m_method(cb.m_method),
        m_param1(cb.m_param1), m_param2(cb.m_param2), m_param3(cb.m_param3), m_param4(cb.m_param4)
        {
        }
    };
    
    template<class Class, typename ParamType1, typename ParamType2, typename ParamType3>
    class _Callback<Class, ParamType1, ParamType2, ParamType3>
    {
    protected:
        
        typedef void (Class::*Method)(ParamType1, ParamType2, ParamType3);
        Class* m_object;
        Method m_method;
        ParamType1 m_param1;
        ParamType2 m_param2;
        ParamType3 m_param3;
        void _Execute() { (m_object->*m_method)(m_param1, m_param2, m_param3); }
        
    public:
        _Callback(Class* object, Method method, ParamType1 param1, ParamType2 param2, ParamType3 param3)
        : m_object(object), m_method(method),
        m_param1(param1), m_param2(param2), m_param3(param3)
        {
        }
        
        _Callback(_Callback<Class, ParamType1, ParamType2, ParamType3> const& cb)
        : m_object(cb.m_object), m_method(cb.m_method),
        m_param1(cb.m_param1), m_param2(cb.m_param2), m_param3(cb.m_param3)
        {
        }
    };
    
    template<class Class, typename ParamType1, typename ParamType2>
    class _Callback<Class, ParamType1, ParamType2>
    {
    protected:
        
        typedef void (Class::*Method)(ParamType1, ParamType2);
        Class* m_object;
        Method m_method;
        ParamType1 m_param1;
        ParamType2 m_param2;
        void _Execute() { (m_object->*m_method)(m_param1, m_param2); }
        
    public:
        
        _Callback(Class* object, Method method, ParamType1 param1, ParamType2 param2)
        : m_object(object), m_method(method),
        m_param1(param1), m_param2(param2)
        {
        }
        
        _Callback(_Callback<Class, ParamType1, ParamType2> const& cb)
        : m_object(cb.m_object), m_method(cb.m_method),
        m_param1(cb.m_param1), m_param2(cb.m_param2)
        {
        }
    };
    
    template<class Class, typename ParamType1>
    class _Callback<Class, ParamType1>
    {
    protected:
        
        typedef void (Class::*Method)(ParamType1);
        Class* m_object;
        Method m_method;
        ParamType1 m_param1;
        void _Execute() { (m_object->*m_method)(m_param1); }
        
    public:
        
        _Callback(Class* object, Method method, ParamType1 param1)
        : m_object(object), m_method(method),
        m_param1(param1)
        {
        }
        
        _Callback(_Callback<Class, ParamType1> const& cb)
        : m_object(cb.m_object), m_method(cb.m_method),
        m_param1(cb.m_param1)
        {
        }
    };
    
    template<class Class>
    class _Callback<Class>
    {
    protected:
        
        typedef void (Class::*Method)();
        Class* m_object;
        Method m_method;
        void _Execute() { (m_object->*m_method)(); }
        
    public:
        _Callback(Class* object, Method method)
        : m_object(object), m_method(method)
        {
        }
        _Callback(_Callback<Class> const& cb)
        : m_object(cb.m_object), m_method(cb.m_method)
        {
        }
    };
    
    /// ---- Statics ----
    template<typename ParamType1 = void, typename ParamType2 = void, typename ParamType3 = void, typename ParamType4 = void>
    class _SCallback
    {
    protected:
        
        typedef void (*Method)(ParamType1, ParamType2, ParamType3, ParamType4);
        Method m_method;
        ParamType1 m_param1;
        ParamType2 m_param2;
        ParamType3 m_param3;
        ParamType4 m_param4;
        void _Execute() { (*m_method)(m_param1, m_param2, m_param3, m_param4); }
        
    public:
        
        _SCallback(Method method, ParamType1 param1, ParamType2 param2, ParamType3 param3, ParamType4 param4)
        : m_method(method),
        m_param1(param1), m_param2(param2), m_param3(param3), m_param4(param4)
        {
        }
        
        _SCallback(_SCallback<ParamType1, ParamType2, ParamType3, ParamType4> const& cb)
        : m_method(cb.m_method),
        m_param1(cb.m_param1), m_param2(cb.m_param2), m_param3(cb.m_param3), m_param4(cb.m_param4)
        {
        }
    };
    
    template<typename ParamType1, typename ParamType2, typename ParamType3>
    class _SCallback<ParamType1, ParamType2, ParamType3>
    {
    protected:
        
        typedef void (*Method)(ParamType1, ParamType2, ParamType3);
        Method m_method;
        ParamType1 m_param1;
        ParamType2 m_param2;
        ParamType3 m_param3;
        void _Execute() { (*m_method)(m_param1, m_param2, m_param3); }
        
    public:
        _SCallback(Method method, ParamType1 param1, ParamType2 param2, ParamType3 param3)
        : m_method(method),
        m_param1(param1), m_param2(param2), m_param3(param3)
        {
        }
        _SCallback(_SCallback<ParamType1, ParamType2, ParamType3> const& cb)
        : m_method(cb.m_method),
        m_param1(cb.m_param1), m_param2(cb.m_param2), m_param3(cb.m_param3)
        {
        }
    };
    
    template<typename ParamType1, typename ParamType2>
    class _SCallback<ParamType1, ParamType2>
    {
    protected:
        
        typedef void (*Method)(ParamType1, ParamType2);
        Method m_method;
        ParamType1 m_param1;
        ParamType2 m_param2;
        void _Execute() { (*m_method)(m_param1, m_param2); }
        
    public:
        _SCallback(Method method, ParamType1 param1, ParamType2 param2)
        : m_method(method),
        m_param1(param1), m_param2(param2)
        {
        }
        
        _SCallback(_SCallback<ParamType1, ParamType2> const& cb)
        : m_method(cb.m_method),
        m_param1(cb.m_param1), m_param2(cb.m_param2)
        {
        }
    };
    
    template<typename ParamType1>
    class _SCallback<ParamType1>
    {
    protected:
        
        typedef void (*Method)(ParamType1);
        Method m_method;
        ParamType1 m_param1;
        void _Execute() { (*m_method)(m_param1); }
        
    public:
        _SCallback(Method method, ParamType1 param1)
        : m_method(method),
        m_param1(param1)
        {
        }
        
        _SCallback(_SCallback<ParamType1> const& cb)
        : m_method(cb.m_method),
        m_param1(cb.m_param1)
        {
        }
    };
    
    template<>
    class _SCallback<>
    {
    protected:
        
        typedef void (*Method)();
        Method m_method;
        void _Execute() { (*m_method)(); }
        
    public:
        
        _SCallback(Method method)
        : m_method(method)
        {
        }
        
        _SCallback(_SCallback<> const& cb)
        : m_method(cb.m_method)
        {
        }
    };
}

namespace Block
{
    class ICallback : virtual public Task
    {
    public:
        
        virtual void Execute() = 0;
        virtual ~ICallback() {}
        
        void perform()override
        {
            Execute();
        }
    };
    
    template<class CB>
    class _ICallback : public CB, public ICallback
    {
    public:
        _ICallback(CB const& cb) : CB(cb)
        {}
        
        void Execute()override { CB::_Execute(); }
    };
};

namespace Block
{
    template<class Class, typename ParamType1 = void, typename ParamType2 = void, typename ParamType3 = void, typename ParamType4 = void>
    class Callback : public _ICallback<_Callback<Class, ParamType1, ParamType2, ParamType3, ParamType4>>
    {
    private:
        
        typedef _Callback<Class, ParamType1, ParamType2, ParamType3, ParamType4> C4;
    public:
        
        Callback(Class* object, typename C4::Method method, ParamType1 param1, ParamType2 param2, ParamType3 param3, ParamType4 param4)
        : _ICallback<C4>(C4(object, method, param1, param2, param3, param4))
        {
        }
    };
    
    template<class Class, typename ParamType1, typename ParamType2, typename ParamType3>
    class Callback<Class, ParamType1, ParamType2, ParamType3> : public _ICallback<_Callback<Class, ParamType1, ParamType2, ParamType3>>
    {
    private:
        
        typedef _Callback<Class, ParamType1, ParamType2, ParamType3> C3;
    public:
        
        Callback(Class* object, typename C3::Method method, ParamType1 param1, ParamType2 param2, ParamType3 param3)
        : _ICallback<C3>(C3(object, method, param1, param2, param3))
        {
        }
    };
    
    template<class Class, typename ParamType1, typename ParamType2>
    class Callback<Class, ParamType1, ParamType2> : public _ICallback<_Callback<Class, ParamType1, ParamType2>>
    {
    private:
        
        typedef _Callback<Class, ParamType1, ParamType2> C2;
        
    public:
        Callback(Class* object, typename C2::Method method, ParamType1 param1, ParamType2 param2)
        : _ICallback<C2>(C2(object, method, param1, param2))
        {
        }
    };
    
    template<class Class, typename ParamType1>
    class Callback<Class, ParamType1> : public _ICallback<_Callback<Class, ParamType1>>
    {
    private:
        
        typedef _Callback<Class, ParamType1> C1;
        
    public:
        
        Callback(Class* object, typename C1::Method method, ParamType1 param1)
        : _ICallback<C1>(C1(object, method, param1))
        {
        }
    };
    
    template<class Class>
    class Callback<Class> : public _ICallback<_Callback<Class> >
    {
    private:
        
        typedef _Callback<Class> C0;
        
    public:
        
        Callback(Class* object, typename C0::Method method)
        : _ICallback<C0>(C0(object, method))
        {
        }
    };
}


namespace Block
{
    template<typename ParamType1 = void, typename ParamType2 = void, typename ParamType3 = void, typename ParamType4 = void>
    class SCallback : public _ICallback<_SCallback<ParamType1, ParamType2, ParamType3, ParamType4> >
    {
    private:
        
        typedef _SCallback<ParamType1, ParamType2, ParamType3, ParamType4> C4;
    public:
        
        SCallback(typename C4::Method method, ParamType1 param1, ParamType2 param2, ParamType3 param3, ParamType4 param4)
        : _ICallback<C4>(C4(method, param1, param2, param3, param4))
        {
        }
    };
    
    template<typename ParamType1, typename ParamType2, typename ParamType3>
    class SCallback<ParamType1, ParamType2, ParamType3> : public _ICallback<_SCallback<ParamType1, ParamType2, ParamType3>>
    {
    private:
        
        typedef _SCallback<ParamType1, ParamType2, ParamType3> C3;
    public:
        
        SCallback(typename C3::Method method, ParamType1 param1, ParamType2 param2, ParamType3 param3)
        : _ICallback<C3>(C3(method, param1, param2, param3))
        {
        }
    };
    
    template<typename ParamType1, typename ParamType2>
    class SCallback<ParamType1, ParamType2> : public _ICallback<_SCallback<ParamType1, ParamType2>>
    {
    private:
        
        typedef _SCallback<ParamType1, ParamType2> C2;
        
    public:
        SCallback(typename C2::Method method, ParamType1 param1, ParamType2 param2)
        : _ICallback<C2>(C2(method, param1, param2))
        {
        }
    };
    
    template<typename ParamType1>
    class SCallback<ParamType1> : public _ICallback<_SCallback<ParamType1> >
    {
    private:
        
        typedef _SCallback<ParamType1> C1;
        
    public:
        
        SCallback(typename C1::Method method, ParamType1 param1)
        : _ICallback<C1>(C1(method, param1))
        {
        }
    };
    
    template<>
    class SCallback<> : public _ICallback<_SCallback<> >
    {
    private:
        
        typedef _SCallback<> C0;
        
    public:
        
        SCallback(typename C0::Method method)
        : _ICallback<C0>(C0(method))
        {
        }
    };
}


#endif /* task_base_h */
