//
//  land_paper.hpp
//  ServerPlugIn
//  玩家每次操作的记录
//  Created by MikeRiy on 16/8/16.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef land_paper_h
#define land_paper_h

#include "land_analyse.h"

class LandPaper
{
public:
    int8 action_type;                   //操作类型
    CARD_TYPE card_type;                //牌型
public:
    SEAT_T oper_id;                     //操作的座位
    int8 card_size;                     //牌长度
    int type_size;                      //牌型长度
public:
    CARD_T cards[LANDLORD_HAND_MAX];    //操作的牌
    
public:
    LandPaper();
    
    virtual ~LandPaper();
    
    void init();
    
    //超过(长度必须大于1)
    bool outPress(CARD_T* gcards, int8 size, CARD_TYPE ctype, int tsize);
};

#endif /* land_paper_hpp */
