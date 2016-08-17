//
//  land_seat.hpp
//  ServerPlugIn
//  斗地主，座位用户数据(零时)
//  Created by MikeRiy on 16/8/15.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef land_seat_h
#define land_seat_h

#include "../global.h"
#include "land_analyse.h"

class LandlordLogic;

class LandSeat
{
public:
    SEAT_T seat_id;
    USER_T user_id;
public:
    int8 sub_size;          //出过的牌，位移到最末端
    STATUS_T state;         //座位状态
public:
    CARD_T opens[LANDLORD_HAND_MAX];
    
public:
    LandSeat();
    
    void init(SEAT_T seatid, USER_T userid);
    
    //放入关闭列表
    void discard(int8* nums, int8 size);
    
    bool isEmpty();
    
    virtual ~LandSeat();
};


#endif /* land_seat_h */
