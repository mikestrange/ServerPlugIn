//
//  lock_base.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/7/30.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "lock.h"

//class lock
Locked::Locked()
:is_locked(false)
{
    pthread_mutex_init(&m_lock, 0);
}

Locked::~Locked()
{
    while(is_locked);
    unlock();
    pthread_mutex_destroy(&m_lock);
}

void Locked::lock()
{
    pthread_mutex_lock(&m_lock);
    is_locked = true;
}

void Locked::unlock()
{
    is_locked = false;
    pthread_mutex_unlock(&m_lock);
}

pthread_mutex_t* Locked::getLocked()
{
    return &m_lock;
}

