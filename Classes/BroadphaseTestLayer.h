//
//  BroadphaseTestLayer.h
//  PhysicsProj
//
//  Created by 张唯 on 16/3/29.
//
//

#ifndef BroadphaseTestLayer_h
#define BroadphaseTestLayer_h

#include "cocos2d.h"
#include "MyPhysics.h"

USING_NS_CC;

class MyContactListenerImpl3;

class BroadphaseTestLayer: public cocos2d::CCLayer
{
public:
    CREATE_FUNC(BroadphaseTestLayer);
    BroadphaseTestLayer();
    ~BroadphaseTestLayer();
    void update(float dt);
    virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
    
protected:
    virtual bool init();
    virtual void draw();
    virtual void onEnter();
private:
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    
    void addNewSpriteAtPosition(CCPoint p);
    void initPhysics();
    void createResetButton();
    void reset(CCObject* sender);
    
private:
    CCTexture2D* m_pSpriteTexture;    // weak ref
    MyWorld* m_pWorld;
    int m_counter;
    MyContactListenerImpl3* m_pListener;
};

class MyContactListenerImpl3: public MyContactListener
{
public:
    MyContactListenerImpl3(){};
    ~MyContactListenerImpl3(){};
    virtual void BeginContact(MyContact* contact);
    virtual void EndContact(MyContact* contact);
};


#endif /* BroadphaseTestLayer_h */
