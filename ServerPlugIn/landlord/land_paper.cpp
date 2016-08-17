//
//  land_paper.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/16.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "land_paper.h"

LandPaper::LandPaper()
:action_type(0)
,oper_id(0)
,card_type(0)
,card_size(0)
,type_size(0){}

LandPaper::~LandPaper()
{
    
}

void LandPaper::init()
{
    action_type = 0;
    oper_id = 0;
    card_type = 0;
    card_size = 0;
    type_size = 0;
}

bool LandPaper::outPress(CARD_T *gcards, int8 size, CARD_TYPE ctype, int tsize)
{
    bool is_ok = false;
    if(action_type == 0){//未操作过
        is_ok = true;
    }else if(LAND_GHOST_BOMB == ctype){//王炸
        is_ok = true;
    }else if(ctype == card_type && type_size == tsize){//同类型
        if(Landlords::BitValue(gcards[0]) > Landlords::BitValue(cards[0]))
        {
            is_ok = true;
        }
    }else if(card_type < LAND_FOUR_BOMB && ctype >= LAND_FOUR_BOMB){//非炸弹
        is_ok = true;
    }
    //更新操作
    if(is_ok)
    {
        card_size = size;
        card_type = ctype;
        type_size = tsize;
        action_type = LANDLORD_OPER_FOLLOW;
        //替换牌
        for(int8 i = 0; i < size; i++)
        {
            cards[i] = gcards[i];
        }
    }
    return is_ok;
}
