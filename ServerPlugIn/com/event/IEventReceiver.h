//
//  IEventReceiver.hpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/9/7.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef IEventReceiver_hpp
#define IEventReceiver_hpp

#include <vector>
#include "lock.h"

typedef unsigned int EVENT_TYPE;

class EventObserver;
class IEventReceiver;
class Event;

//事件
class Event
{
private:
    EVENT_TYPE event_type;
    int event_tag;
    IEventReceiver* receiver;
public:
    Event(EVENT_TYPE etype, int etag = 0)
    :event_type(etype)
    ,event_tag(etag)
    ,receiver(NULL)
    {};
    
    friend class IEventReceiver;
    void target(IEventReceiver* value)
    {
        receiver = value;
    }
    
    IEventReceiver* target()
    {
        return receiver;
    }
    
    EVENT_TYPE type(){ return event_type;}
    
    int tag(){  return event_tag;}
    
    virtual ~Event(){   }
};

//事件观察(调用真空，所以加索引)
class EventObserver : public RefObject
{
private:
    IEventReceiver* Receiver;
public:
    EventObserver(IEventReceiver* target)
    :Receiver(target)
    {}
    
    virtual ~EventObserver()
    {}
    
    void Destroy()
    {
        Receiver = NULL;
    }
    
    bool MatchTarget(IEventReceiver* target)
    {
        return Receiver == target;
    }
    
    void OnEvent(Event* event);
};

//观察队列
class ListObserver
{
private:
    std::vector<EventObserver*> m_list;
public:
    ListObserver(){};
    
    virtual ~ListObserver(){};
    
    //不添加同一接受者
    bool AddReceiver(IEventReceiver* receiver)
    {
        if(HasReceiver(receiver))
        {
            return false;
        }
        m_list.push_back(new EventObserver(receiver));
        return true;
    }
    
    bool RemoveReceiver(IEventReceiver* receiver)
    {
        std::vector<EventObserver*>::iterator iter;
        for(iter = m_list.begin(); iter != m_list.end(); iter++)
        {
            EventObserver* observer = *iter;
            if(observer->MatchTarget(receiver))
            {
                m_list.erase(iter);
                observer->Destroy();
                observer->release();
                break;
            }
        }
        return m_list.empty();
    }
    
    bool HasReceiver(IEventReceiver* receiver)
    {
        for(int i = 0; i < m_list.size(); i++)
        {
            if(m_list[i]->MatchTarget(receiver))
            {
                return true;
            }
        }
        return false;
    }
    
    void ApplyReceivers(std::vector<EventObserver*> &handle_list)
    {
        for(int i = 0; i < m_list.size(); i++)
        {
            handle_list.push_back(m_list[i]);
            m_list[i]->retain();
        }
    }
};

//全局派送
void dispatch_event(Event* event);

//事件接受者(全局的)
class IEventReceiver
{
public:
    virtual void OnEventProcessor(Event* event)=0;
public:
    //建立自己的监听
    EVENT_TYPE CreateEventListener();
    //自定义
    EVENT_TYPE AddEventListener(EVENT_TYPE type);
    
    void RemoveEventListener(EVENT_TYPE type);
    
    void DispatchEvent(Event* event);
    
    bool HasEventListener(EVENT_TYPE type);
};

#endif /* IEventReceiver_hpp */
