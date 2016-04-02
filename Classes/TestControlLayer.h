//
//  TestControllLayer.h
//  PhysicsProj
//
//  Created by 张唯 on 16/3/28.
//
//

#ifndef TestControlLayer_h
#define TestControlLayer_h

#include "cocos2d.h"

USING_NS_CC;

class TestControlLayer: public cocos2d::CCLayer
{
public:
    CREATE_FUNC(TestControlLayer);
    TestControlLayer();
    ~TestControlLayer();
    
    void menuCallback(CCObject* pSender);
    void closeCallback(CCObject * pSender);
protected:
    CCLayer* createTestLayer(int index);
    virtual bool init();

private:
    CCMenu* m_pItemMenu;
    
    
};

#endif /* TestControllLayer_h */
