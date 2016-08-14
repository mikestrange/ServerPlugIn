//
//  landlord_logic.h
//  ServerPlugIn
//  斗地主逻辑
//  Created by MikeRiy on 16/8/13.
//  Copyright © 2016年 MikeRiy. All rights reserved.
//

#ifndef landlord_logic_h
#define landlord_logic_h

#include "logic_base.h"


class LandlordLogic : public LogicBase
{
public:
    LandlordLogic();
    virtual ~LandlordLogic();
public:
    void launch()override;
    void revoke()override;
    
private:
    
};

#endif /* landlord_logic_h */
