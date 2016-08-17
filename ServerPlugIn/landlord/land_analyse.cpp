//
//  land_analyse.cpp
//  ServerPlugIn
//
//  Created by MikeRiy on 16/8/15.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#include "land_analyse.h"


static void SortValue(CARD_T *cards, int32 length)
{
    utils::SortOn(cards, length, [](CARD_T a, CARD_T b){
        if(Landlords::BitValue(a) > Landlords::BitValue(b)){
            return 1;
        }
        return -1;
    });
}

//是否存在不能连续的值
static bool isBrock(CARD_T *cards, int32 size)
{
    for(int i = 0; i < size; i++)
    {
        //不能为2，不能为鬼
        if(Landlords::BitValue(cards[i]) > CARD_A)
        {
            return true;
        }
    }
    return false;
}

//4个是否相等
static bool isFours(CARD_T a1, CARD_T a2, CARD_T a3, CARD_T a4)
{
    return a1 == a2 && a2 == a3 && a3 == a4;
}

//3个是否相等
static bool isThrees(CARD_T a1, CARD_T a2, CARD_T a3)
{
    return a1 == a2 && a2 == a3;
}

//判断是否存在glen条相同，存在将glen条放入最前面(如果查看连续性，先按大小排列)
static bool isHaveSame(CARD_T *cards, int32 pos, int32 size, int8 glen)
{
    if(pos >= size) return false;
    CARD_T temp = 0;
    int32 clen = 0;
    int32 t_pos[glen];//连续位置
    for(int32 i = pos; i < size; i++)
    {
        CARD_T v = Landlords::BitValue(cards[i]);
        if(v == temp){
            clen++;
            t_pos[clen - 1] = i;
            if(clen == glen) break;
        }else{
            clen = 1;
            t_pos[clen - 1] = i;
            temp = v;
        }
    }
    //出现N条
    if(clen == glen)
    {
        //置顶
        for(int i = 0; i < clen; i++)
        {
            if(pos + i == t_pos[i]) continue;
            CARD_T v = cards[pos + i];
            cards[pos + i] = cards[t_pos[i]];
            cards[t_pos[i]] = v;
        }
        return true;
    }
    return false;
}

namespace Landlords
{
    CARD_T BitValue(CARD_T card)
    {
        return card&0xff;
    }
    
    CARD_T BitColor(CARD_T card)
    {
        return card>>8;
    }
    
    
    int match(CARD_T *cards, int32 size, CARD_TYPE type)
    {
        if(size > LANDLORD_HAND_MAX) return -1;
        //牌型
        switch(type)
        {
            case LAND_NONE:
                return 1;
                break;//单张
            case LAND_ONE_PAIRS:
                return Landlords::one_pairs(cards, size);
                break;//对子
            case LAND_EVEN_PAIRS:
                return Landlords::even_pairs(cards, size);
                break;//连对
            case LAND_THREE_ONLY:
                return Landlords::three_only(cards, size);
                break;//三条
            case LAND_THREE_WITH_ONE:
                return Landlords::three_with_one(cards, size);
                break;//三带一
            case LAND_THREE_WITH_PAIR:
                return Landlords::three_with_pair(cards, size);
                break;//三带一对
            case LAND_PLANE_ONLY:
                return Landlords::threes_only(cards, size);
                break;//飞机
            case LAND_PLANE_WITH_ONE:
                return Landlords::threes_with_one(cards, size);
                break;//飞机带单张
            case LAND_PLANE_WITH_PAIR:
                return Landlords::threes_with_pair(cards, size);
                break;//飞机带对子
            case LAND_STRAIGHT:
                return Landlords::straight(cards, size);
                break;//顺子
            case LAND_FOUR_WITH_ONE:
                return Landlords::four_with_one(cards, size);
                break;//四带2
            case LAND_FOUR_WITH_PAIRS:
                return Landlords::four_with_pair(cards, size);
                break;//四带两对
            case LAND_FOUR_BOMB:
                return Landlords::four_card(cards, size);
                break;//炸弹
            case LAND_GHOST_BOMB:
                return Landlords::ghost_pair(cards, size);
                break;//王炸
        }
        return -1;
    }
    
    //1对
    int one_pairs(CARD_T *cards, int32 length)
    {
        if(length==2 && BitValue(cards[0]) == BitValue(cards[1]))
        {
            return 1;
        }
        return -1;
    }
    
    //连对(比较第一张谁大就可以了)
    int even_pairs(CARD_T *cards, int32 length)
    {
        //确保3对
        if(length < 6) return -1;
        //是否双数
        if(length%2 > 0) return -1;
        //不能存在2和鬼
        if(isBrock(cards, length)) return -1;
        //值排序
        SortValue(cards, length);
        //是否全对子
        for(int i = 0; i < length/2; i++)
        {
            if(BitValue(cards[i*2]) != BitValue(cards[i*2 + 1]))
            {
                return -1;
            }
        }
        //都是对，那么看连续性
        for(int i = 1; i < length/2; i++)
        {
            if(BitValue(cards[(i-1)*2]) + 1 != BitValue(cards[i*2]))
            {
                return -1;
            }
        }
        return length/2;
    }
    
    
    //3不带(比较第一张谁大就可以了)
    int three_only(CARD_T *cards, int32 length)
    {
        if(length == 3 && isThrees(BitValue(cards[0]), BitValue(cards[1]), BitValue(cards[2])))
        {
            return 1;
        }
        return -1;
    }
    
    //3带1(比较第一张谁大就可以了)
    int three_with_one(CARD_T *cards, int32 length)
    {
        if(length == 4)
        {
            if(isHaveSame(cards, 0, length, 3))
            {
                return 1;
            }
        }
        return -1;
    }
    
    //3带1对(比较第一张谁大就可以了)
    int three_with_pair(CARD_T *cards, int32 length)
    {
        if(length == 5)
        {
            if(isHaveSame(cards, 0, length, 3) && BitValue(cards[3]) == BitValue(cards[4]))
            {
                return 1;
            }
        }
        return -1;
    }
    
    //飞机不带(返回3条个数>1)(比较第一张谁大就可以了)
    int threes_only(CARD_T *cards, int32 length)
    {
        if(length < 6) return -1;
        if(length%3 > 0) return -1;
        //存在非连续元素
        if(isBrock(cards, length)) return -1;
        //小->大排序
        SortValue(cards, length);
        //查看是否都为3条
        for(int i = 0; i<length/3; i++)
        {
            CARD_T v1 = BitValue(cards[i*3]);
            CARD_T v2 = BitValue(cards[i*3 + 1]);
            CARD_T v3 = BitValue(cards[i*3 + 2]);
            if(v1 == v2 && v2 == v3){
                
            }else{
                return -1;
            }
        }
        //连续性
        for(int i = 1; i < length/3; i++)
        {
            if(BitValue(cards[(i-1)*3]) + 1 != BitValue(cards[i*3]))
            {
                return -1;
            }
        }
        return length/3;
    }
    
    //飞机带单(返回3条个数>1)(比较第一张谁大就可以了)
    int threes_with_one(CARD_T *cards, int32 length)
    {
        if(length < 8) return -1;
        if(length%4 > 0) return -1;
        //存在非连续元素
        if(isBrock(cards, length)) return -1;
        //小->大排序
        SortValue(cards, length);
        //
        int32 pos = 0;
        int32 tlen = 0;
        while(isHaveSame(cards, pos, length, 3))
        {
            tlen++;
            pos += 3;
        }
        //判断连续性
        for(int i = 1; i < tlen; i++)
        {
            if(BitValue(cards[(i-1)*3]) + 1 != BitValue(cards[i*3]))
            {
                return -1;
            }
        }
        if(tlen < 2) return -1;
        return tlen;
    }
    
    //飞机带1对(返回3条个数>1)(比较第一张谁大就可以了)
    int threes_with_pair(CARD_T *cards, int32 length)
    {
        if(length < 10) return -1;
        if(length%5 > 0) return -1;
        //存在非连续元素
        if(isBrock(cards, length)) return -1;
        //小->大排序
        SortValue(cards, length);
        //
        int32 pos = 0;
        int32 tlen = 0;
        while(isHaveSame(cards, pos, length, 3))
        {
            tlen++;
            pos += 3;
        }
        //判断连续性
        for(int i = 1; i < tlen; i++)
        {
            if(BitValue(cards[(i-1)*3]) + 1 != BitValue(cards[i*3]))
            {
                return -1;
            }
        }
        //查看对子是否完整(已经排序)
        int p = tlen*3;
        int plen = (length - p)/2;
        for(int i = 0; i < plen; i++)
        {
            if(BitValue(cards[p + i*2]) != BitValue(cards[p + i*2 + 1]))
            {
                return -1;
            }
        }
        if(tlen < 2) return -1;
        return tlen;
    }
    
    //顺子(放回顺子长度 >=5)(比较第一张谁大就可以了)
    int straight(CARD_T *cards, int32 length)
    {
        if(length < 5) return false;
        //小->大排序
        SortValue(cards, length);
        //存在非连续元素
        if(isBrock(cards, length)) return -1;
        //判断连续性
        for(int i = 1; i<length; i++)
        {
            if(BitValue(cards[i-1]) + 1 != BitValue(cards[i]))
            {
                return -1;
            }
        }
        return length;
    }
    
    //4条(比较第一张谁大就可以了)
    int four_card(CARD_T *cards, int32 length)
    {
        if(length == 4 && isFours(BitValue(cards[0]), BitValue(cards[1]), BitValue(cards[2]), BitValue(cards[3])))
        {
            return 1;
        }
        return -1;
    }
    
    //4带2对(4条放前面来)(比较第一张谁大就可以了)
    int four_with_pair(CARD_T *cards, int32 length)
    {
        if(length == 8)
        {
            if(isHaveSame(cards, 0, length, 4) && isHaveSame(cards, 4, length, 2) &&
               isHaveSame(cards, 6, length, 2))
            {
                return 1;
            }
        }
        return -1;
    }
    
    //4带2张(2张可以是一对)(比较第一张谁大就可以了)
    int four_with_one(CARD_T *cards, int32 length)
    {
        if(length == 6)
        {
            //存在4条就可以了
            if(isHaveSame(cards, 0, length, 4))
            {
                return 1;
            }
        }
        return -1;
    }
    
    //王炸
    int ghost_pair(CARD_T *cards, int32 length)
    {
        if(length == 2 && BitColor(cards[0]) == COLOR_5 && BitColor(cards[1]) == COLOR_5)
        {
            return 1;
        }
        return -1;
    }
}