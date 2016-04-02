#ifndef __START_LAYER_H__
#define __START_LAYER_H__

#include "cocos2d.h"
#include "MyPhysics.h"

USING_NS_CC;

class MyContactListenerImpl;

class NarrowphaseTestLayer : public cocos2d::CCLayer
{
public:
    CREATE_FUNC(NarrowphaseTestLayer);
    NarrowphaseTestLayer();
    ~NarrowphaseTestLayer();
    void update(float dt);
    virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
    virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);
    virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
protected:
    virtual bool init();
    virtual void draw();
    virtual void onEnter();
private:
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    
    void addNewSpriteAtPosition(CCPoint p);
    void addNewSpriteAtPosition2(CCPoint p);
    void initPhysics();
    void createResetButton();
    void reset(CCObject* sender);
    
private:
    CCTexture2D* m_pSpriteTexture;    // weak ref
    MyWorld* m_pWorld;
    MyBody* m_pHoldingBody;
    int m_counter;
    MyContactListenerImpl* m_pListener;
};

class MyContactListenerImpl: public MyContactListener
{
public:
    MyContactListenerImpl(){};
    ~MyContactListenerImpl(){};
    virtual void BeginContact(MyContact* contact);
    virtual void EndContact(MyContact* contact);
};
#endif //__START_LAYER_H__
