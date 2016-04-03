//
//  GameLayer.h
//  PhysicsProj
//
//  Created by 张唯 on 16/4/2.
//
//

#ifndef GameLayer_h
#define GameLayer_h


#include "cocos2d.h"
#include "MyPhysics.h"
#include "MinipoolBall.h"
#include "MinipoolCue.h"
#include "LineContainer.h"

USING_NS_CC;

class MyContactListenerImpl4;

class GameLayer: public cocos2d::CCLayer
{
public:
    CREATE_FUNC(GameLayer);
    GameLayer();
    ~GameLayer();
    void update(float dt);
    virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
    virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
    virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);
    
    MyWorld* getWorld() {return m_pWorld;}
    
protected:
    virtual bool init();
    virtual void draw();
    virtual void onEnter();
private:
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    
    void initPhysics();
    void createResetButton();
    void reset(CCObject* sender);
    void placeCue(CCPoint pos);
private:
    float m_pullBack;
    CCPoint m_cueStartPoint;
    CCTouch *m_touch;
    LineContainer *m_lineContainer;
    MinipoolBall* m_playerBall;
    MinipoolCue* m_cue;
    CCSpriteBatchNode *m_gameBatchNode;
    MyWorld* m_pWorld;
    MyContactListenerImpl4* m_pListener;
};

class MyContactListenerImpl4: public MyContactListener
{
public:
    MyContactListenerImpl4(){};
    ~MyContactListenerImpl4(){};
    virtual void BeginContact(MyContact* contact);
    virtual void EndContact(MyContact* contact);
};


#endif /* GameLayer_h */
