//
//  land_player.hpp
//  ServerPlugIn
//  斗地主 房间用户数据
//  Created by MikeRiy on 16/8/15.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef land_player_h
#define land_player_h

#include "../global.h"

class LandPlayer
{
private:
    USER_T user_id;
    SEAT_T seat_id;
    
public:
    LandPlayer(USER_T uid);
    
    virtual~ LandPlayer();
    
    bool isSit()const;
    
    void Stand();
    
    bool SitDown(SEAT_T value);
    
    bool isSeatId(SEAT_T value);
    
    SEAT_T getSeatId()const;
    
    USER_T getUserId()const;
};

#endif /* land_player_hpp */
