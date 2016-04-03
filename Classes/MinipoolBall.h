//
//  MinipoolBall.h
//  PhysicsProj
//
//  Created by 张唯 on 16/4/2.
//
//

#ifndef MinipoolBall_h
#define MinipoolBall_h

#include "cocos2d.h"
#include "cocos-ext.h"
using cocos2d::CCSprite;
using cocos2d::extension::CCPhysicsSprite;
using cocos2d::CCPoint;
using cocos2d::CCLayer;

#define BALL_RADIUS 7.0f 

enum{
    enColorYellow,
    enColorRed,
    enColorBlack,
    enColorWhite
};

class MinipoolBall : public CCPhysicsSprite {
public:
    ~MinipoolBall();
    MinipoolBall(CCLayer *game, CCPoint position, int color);
    static MinipoolBall* create(CCLayer *game, CCPoint position, int color);
    void reset();

private:
    void initBall();
    CCPoint m_startPosition;
    CCLayer *m_pGameLayer;
    bool m_bInPlay;
    int m_color;
};


#endif /* MinipoolBall_h */
