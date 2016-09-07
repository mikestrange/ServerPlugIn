//
//  IEventReceiver.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/9/7.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "IEventReceiver.h"

#include "hash_map.h"

static HashMap<EVENT_TYPE, ListObserver*> event_map;
static EVENT_TYPE current_type;
static const EVENT_TYPE NONE_EVENT = 0;

//保证消息的唯一性
static EVENT_TYPE listen_event(EVENT_TYPE etype, IEventReceiver* receiver)
{
    EVENT_TYPE type = NONE_EVENT;
    if(etype > NONE_EVENT){
        auto m_list = event_map.find(etype);
        if(m_list)
        {
            if(m_list->AddReceiver(receiver))
            {
                type = etype;
            }
        }else{
            type = etype;
            m_list = event_map.put(type, new ListObserver);
            m_list->AddReceiver(receiver);
        }
    }else{
        while(event_map.has(++current_type))
        {
            if(current_type == MAX_UINT32) current_type = NONE_EVENT;
        }
        type = current_type;
        auto list = event_map.put(type, new ListObserver);
        list->AddReceiver(receiver);
    }
    return type;
}


static void remove_event(EVENT_TYPE type, IEventReceiver* receiver)
{
    if(type == NONE_EVENT) return;
    //
    auto list = event_map.find(type);
    if(list && list->RemoveReceiver(receiver))
    {
        event_map.remove(type);
        SAFE_DELETE(list);
    }
}

static bool has_event(EVENT_TYPE type, IEventReceiver* receiver)
{
    if(type == NONE_EVENT) return false;
    auto list = event_map.find(type);
    return list && list->HasReceiver(receiver);
}

//good boy
void dispatch_event(Event* event)
{
    auto list = event_map.find(event->type());
    if(list)
    {
        std::vector<EventObserver*> handle_list;
        list->ApplyReceivers(handle_list);
        //handler
        for(int i = 0; i < handle_list.size(); i++)
        {
            handle_list[i]->OnEvent(event);
            handle_list[i]->release();
        }
    }
}


//class EventObserver (do something)
void EventObserver::OnEvent(Event *event)
{
    if(Receiver)
    {
        Receiver->OnEventProcessor(event);
    }
}


//class IEventReceiver
EVENT_TYPE IEventReceiver::CreateEventListener()
{
    return listen_event(NONE_EVENT, this);
}

//不存在就会生成
EVENT_TYPE IEventReceiver::AddEventListener(EVENT_TYPE type)
{
    if(type == NONE_EVENT) return NONE_EVENT;
    return listen_event(type, this);
}

void IEventReceiver::RemoveEventListener(EVENT_TYPE type)
{
    remove_event(type, this);
}

void IEventReceiver::DispatchEvent(Event* event)
{
    event->target(this);
    dispatch_event(event);
}

bool IEventReceiver::HasEventListener(EVENT_TYPE type)
{
    return has_event(type, this);
}