//
//  LineContainer.h
//  PhysicsProj
//
//  Created by 张唯 on 16/4/3.
//
//

#ifndef LineContainer_h
#define LineContainer_h

#include "cocos2d.h"

USING_NS_CC;

class LineContainer : public CCNode{
public:
    CC_SYNTHESIZE(CCPoint,m_ballPoint,BallPoint);
    CC_SYNTHESIZE(CCPoint,m_cuePoint,CuePoint);
    CC_SYNTHESIZE(bool,m_drawing,Drawing);
    
    LineContainer();
    //static LineContainer *create();
    CREATE_FUNC(LineContainer);
    virtual void draw();
    
private:
    int m_dash;
    int m_dashSpace;
    CCSize m_screenSize;
};

#endif /* LineContainer_h */
