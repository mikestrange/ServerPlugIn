//
//  land_seat.hpp
//  ServerPlugIn
//  斗地主，座位用户数据(零时)
//  Created by MikeRiy on 16/8/15.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef land_seat_h
#define land_seat_h

#include "../../com/global.h"
#include "land_analyse.h"

class LandlordLogic;

class LandSeat
{
public:
    SEAT_T seat_id;
    USER_T user_id;
private:
    STATUS_T state;         //座位状态
    int8 card_size;         //总数
    int8 out_size;          //出过的牌，位移到最末端
public:
    CARD_T opens[LANDLORD_HAND_MAX];
    
public:
    LandSeat(SEAT_T seatid, USER_T userid);
    
    virtual ~LandSeat();
    
    void init();
    
    void setCardSize(int8 size);
    
    int8 getSubSize();
    
    int8 getCardSize()const;
    
    //是状态
    bool BeState(STATUS_T value);
    
    //设置状态
    void SetState(STATUS_T value);
    
    //放入关闭列表
    bool Discard(int8* nums, int8 size);
};


#endif /* land_seat_h */
