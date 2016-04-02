//
//  ContactSolverTestLayer.h
//  PhysicsProj
//
//  Created by 张唯 on 16/3/27.
//
//

#ifndef ContactSolverTestLayer_h
#define ContactSolverTestLayer_h

#include "cocos2d.h"
#include "MyPhysics.h"

USING_NS_CC;

class MyContactListenerImpl2;

class ContactSolverTestLayer : public cocos2d::CCLayer
{
public:
    CREATE_FUNC(ContactSolverTestLayer);
    ContactSolverTestLayer();
    ~ContactSolverTestLayer();
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
    MyBody* m_pHoldingBody;
    int m_counter;
    MyContactListenerImpl2* m_pListener;
};

class MyContactListenerImpl2: public MyContactListener
{
public:
    MyContactListenerImpl2(){};
    ~MyContactListenerImpl2(){};
    virtual void BeginContact(MyContact* contact);
    virtual void EndContact(MyContact* contact);
};


#endif /* ContactSolverTestLayer_h */
