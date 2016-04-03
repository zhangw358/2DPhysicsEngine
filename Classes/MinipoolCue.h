//
//  MinipoolCue.h
//  PhysicsProj
//
//  Created by 张唯 on 16/4/3.
//
//

#ifndef MinipoolCue_h
#define MinipoolCue_h

#include "cocos2d.h"
#include "cocos-ext.h"

using cocos2d::CCLayer;
using cocos2d::extension::CCPhysicsSprite;

class MinipoolCue : public CCPhysicsSprite
{
public:
    ~MinipoolCue();
    MinipoolCue(CCLayer* game);
    static MinipoolCue* create(CCLayer* game);
private:
    void initCue();
private:
    CCLayer *m_pLayer;
};

#endif /* MinipoolCue_h */
