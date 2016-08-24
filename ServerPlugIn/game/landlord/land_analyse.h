//
//  land_analyse.h
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/15.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef land_analyse_h
#define land_analyse_h

#include "../../com/global.h"
#include "../../com/buffer_base.h"
#include "../../com/sort_utils.h"
#include "../../com/string_utils.h"

typedef uint16 CARD_T;
typedef uint8 CARD_TYPE;


//游戏流程状态
enum LANDLORD_STATUS
{
    LANDLORD_GAME_STOP = 0,
    LANDLORD_GAME_START,    //游戏开始
    LANDLORD_ROB_BANKER,    //抢庄中
    LANDLORD_GAME_PLAYER,   //游戏进行中
    LANDLORD_GAME_OVER      //游戏结束
};

//游戏操作状态 (跟或者必出)
enum LANDLORD_OPER_TYPE
{
    LANDLORD_OPER_NONE = 0,     //无操作
    LANDLORD_OPER_FOLLOW,       //跟牌
    LANDLORD_OPER_PASS,         //过牌
};


//玩家状态
enum LANDLORD_USER_STATUE
{
    LANDLORD_USER_ONLINE = 0,
    LANDLORD_USER_LEAVE        //离开(游戏中退出)
};


//座位状态
enum LANDLORD_SEAT_STATUE
{
    LANDLORD_GAME_ONLINE = 0,       //等待游戏(坐下默认)
    LANDLORD_GAME_LEAVE = 1,        //离开了
    LANDLORD_GAME_TLOR = 2          //托管(主动托管)
};

//一副牌
#define LANDLORD_CARD_LEN 54
//发完剩余
#define LANDLORD_SUB_CARD 3
//手牌
#define LANDLORD_HAND_LEN 17
//最多牌
#define LANDLORD_HAND_MAX 20

//斗地主最大倍数
#define LANDLORD_MAX_MULT 3

//方片1，梅花2，红桃3，黑桃4 ,鬼5
#define COLOR_3 0x03
#define COLOR_4 0x04
#define COLOR_5 0x05

//牌的值
#define CARD_3 0x03
#define CARD_4 0x04
#define CARD_5 0x05
#define CARD_6 0x06
#define CARD_7 0x07
#define CARD_8 0x08
#define CARD_9 0x09
#define CARD_10 0x0a
#define CARD_J 0x0b
#define CARD_Q 0x0c
#define CARD_K 0x0d
#define CARD_A 0x0e
#define CARD_2 0x0f
//小王值
#define CARD_G1 0x10
//大王值
#define CARD_G2 0x11

//小王
#define CARD_SMALL_GHOST 0x0510
//大王
#define CARD_BIG_GHOST 0x0511

//一副牌
const CARD_T LANDLORD_CARDS[LANDLORD_CARD_LEN] = {
    0x0103,0x0104,0x0105,0x0106,0x0107,0x0108,0x0109,0x010a,0x010b,0x010c,0x010d,0x010e,0x010f,
    0x0203,0x0204,0x0205,0x0206,0x0207,0x0208,0x0209,0x020a,0x020b,0x020c,0x020d,0x020e,0x020f,
    0x0303,0x0304,0x0305,0x0306,0x0307,0x0308,0x0309,0x030a,0x030b,0x030c,0x030d,0x030e,0x030f,
    0x0403,0x0404,0x0405,0x0406,0x0407,0x0408,0x0409,0x040a,0x040b,0x040c,0x040d,0x040e,0x040f,
    0x0510, 0x0511};

enum LAND_CARD_TYPE
{
    LAND_NONE = 0,              //单张
    LAND_ONE_PAIRS,             //对子
    LAND_EVEN_PAIRS,            //连对
    LAND_THREE_ONLY,            //三条
    LAND_THREE_WITH_ONE,        //三带一
    LAND_THREE_WITH_PAIR,       //三带一对
    LAND_PLANE_ONLY,            //飞机
    LAND_PLANE_WITH_ONE,        //飞机带单张
    LAND_PLANE_WITH_PAIR,       //飞机带对子
    LAND_STRAIGHT,              //顺子
    LAND_FOUR_WITH_ONE,         //四带2
    LAND_FOUR_WITH_PAIRS,       //四带两对
    LAND_FOUR_BOMB,             //炸弹
    LAND_GHOST_BOMB             //王炸
};

namespace Landlords
{
    void SortValue(CARD_T *cards, int32 size);
    
    std::string BitString(CARD_T card);
    
    CARD_T BitValue(CARD_T card);
    
    CARD_T BitColor(CARD_T card);
    
    //-1表示不是
    int match(CARD_T *cards, int32 size, CARD_TYPE type);
    
}

#endif /* land_analyse_h */
