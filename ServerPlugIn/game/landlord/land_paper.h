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
private:
    int8 action_type;                   //操作类型
    CARD_TYPE card_type;                //牌型
private:
    SEAT_T play_id;                     //出牌的座位
    int8 card_size;                     //牌长度
    int type_size;                      //牌型长度
private:
    CARD_T cards[LANDLORD_HAND_MAX];    //操作的牌
    
public:
    LandPaper();
    
    virtual ~LandPaper();
    
    void init();
    
    bool ActionType(int8 atype);
    
    SEAT_T getPlayId()const;
    
    //超过(长度必须大于1)
    bool outPress(SEAT_T handle_id, CARD_T* gcards, int8 size, CARD_TYPE ctype, int tsize);
};

#endif /* land_paper_hpp */
