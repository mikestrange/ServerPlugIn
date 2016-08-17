#include "land_seat.h"

LandSeat::LandSeat()
:seat_id(0)
,user_id(0)
,sub_size(0)
{

}

LandSeat::~LandSeat()
{
    
}

void LandSeat::init(SEAT_T seatid, USER_T userid)
{
    seat_id = seatid;
    user_id = userid;
}
    
    //放入关闭列表
void LandSeat::discard(int8* nums, int8 size)
{
    sub_size -= size;
    for(int8 i = 0; i < size; i++)
    {
        opens[nums[i]] = 0;
    }
}
    
bool LandSeat::isEmpty()
{
    return sub_size <= 0;
}



