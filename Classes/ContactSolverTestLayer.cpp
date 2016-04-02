//
//  ContactSolverTestLayer.cpp
//  PhysicsProj
//
//  Created by 张唯 on 16/3/27.
//
//


#include "StartScene.h"
#include "VisibleRect.h"
#include "cocos-ext.h"
#include "ContactSolverTestLayer.h"

USING_NS_CC;
using extension::CCPhysicsSprite;

enum {
    kTagParentNode = 1,
};

ContactSolverTestLayer::ContactSolverTestLayer()
: m_pSpriteTexture(NULL)
, m_pWorld(NULL)
, m_counter(0)
, m_pListener(NULL)
, m_pHoldingBody(NULL)
{
    
}

ContactSolverTestLayer::~ContactSolverTestLayer()
{
    CC_SAFE_DELETE(m_pWorld);
    CC_SAFE_DELETE(m_pListener);
}

void ContactSolverTestLayer::onEnter()
{
    CCLayer::onEnter();
}

void ContactSolverTestLayer::initPhysics()
{
    m_pWorld = new MyWorld();
    m_pListener = new MyContactListenerImpl2();
    m_pWorld->setContactListener(m_pListener);
    
    MyBody *body = m_pWorld->createBody();
    MyEdgeShape shape1;
    shape1.setPosition(MyVec2(VisibleRect::leftTop().x,VisibleRect::leftTop().y-1), MyVec2(VisibleRect::leftBottom().x,VisibleRect::leftBottom().y+1));
    body->createShape(&shape1);
    
    MyBody *body2 = m_pWorld->createBody();
    MyEdgeShape shape2;
    shape2.setPosition(MyVec2(VisibleRect::leftBottom().x+1,VisibleRect::leftBottom().y), MyVec2(VisibleRect::rightBottom().x-1,VisibleRect::rightBottom().y));
    body2->createShape(&shape2);
    
    MyBody *body3 = m_pWorld->createBody();
    MyEdgeShape shape3;
    shape3.setPosition(MyVec2(VisibleRect::leftTop().x+1,VisibleRect::leftTop().y), MyVec2(VisibleRect::rightTop().x-1,VisibleRect::rightTop().y));
    body3->createShape(&shape3);
    
    MyBody *body4 = m_pWorld->createBody();
    MyEdgeShape shape4;
    shape4.setPosition(MyVec2(VisibleRect::rightBottom().x,VisibleRect::rightBottom().y+1), MyVec2(VisibleRect::rightTop().x,VisibleRect::rightTop().y-1));
    body4->createShape(&shape4);
}


// on "init" you need to initialize your instance
bool ContactSolverTestLayer::init()
{
    //////////////////////////////
    // 1. super init first
    bool bRet = CCLayer::init();
    if ( bRet )
    {
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        
        /////////////////////////////
        // 2. add a menu item with "X" image, which is clicked to quit the program
        //    you may modify it.
        
        // add a "close" icon to exit the progress. it's an autorelease object
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                              "CloseNormal.png",
                                                              "CloseSelected.png",
                                                              this,
                                                              menu_selector(ContactSolverTestLayer::menuCloseCallback));
        
        pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                    origin.y + pCloseItem->getContentSize().height/2));
        
        // create menu, it's an autorelease object
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        this->addChild(pMenu, 1);
        
        /////////////////////////////
        // 3. add your codes below...
        setTouchEnabled( true );
        setAccelerometerEnabled( true );
        
        // init physics
        this->initPhysics();
        // create reset button
        this->createResetButton();
        
        //Set up sprite
        // Use batch node. Faster
        CCSpriteBatchNode *parent = CCSpriteBatchNode::create("Images/blocks.png", 100);
        m_pSpriteTexture = parent->getTexture();
        
        addChild(parent, 0, kTagParentNode);
        
        //addNewSpriteAtPosition(VisibleRect::center());
        
        scheduleUpdate();
    }
    return bRet;
}


void ContactSolverTestLayer::createResetButton()
{
    CCMenuItemImage *reset = CCMenuItemImage::create("Images/r1.png", "Images/r2.png", this, menu_selector(ContactSolverTestLayer::reset));
    
    CCMenu *menu = CCMenu::create(reset, NULL);
    
    menu->setPosition(ccp(VisibleRect::bottom().x, VisibleRect::bottom().y + 30));
    this->addChild(menu, -1);
    
}

void ContactSolverTestLayer::reset(CCObject* sender)
{
    CCDirector::sharedDirector()->replaceScene(StartScene::create());
}

void ContactSolverTestLayer::draw()
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    
    //world->DrawDebugData();
    
    kmGLPopMatrix();
}

void ContactSolverTestLayer::addNewSpriteAtPosition(CCPoint p)
{
    CCLOG("Add sprite %0.2f x %02.f",p.x,p.y);
    
    MyBody *body = m_pWorld->createBody();
    CCPhysicsSprite *sprite = NULL;
    if (m_counter % 2 == 0 )
    {
        MyPolygonShape shape;
        shape.setAsBox(16, 16);
        body->createShape(&shape);
        body->setPosition(MyVec2(p.x, p.y));
        body->setAngularVelocity(0.05f);
        body->setLinearVelocity(MyVec2(5.0f, 5.0f));
        //We have a 64x64 sprite sheet with 4 different 32x32 images.  The following code is
        //just randomly picking one of the images
        int idx = (CCRANDOM_0_1() > .5 ? 0:1);
        int idy = (CCRANDOM_0_1() > .5 ? 0:1);
        sprite = CCPhysicsSprite::createWithTexture(m_pSpriteTexture,CCRectMake(32 * idx,32 * idy,32,32));
        CCNode *parent = this->getChildByTag(kTagParentNode);
        parent->addChild(sprite);
    }
    else
    {
        //body->setLinearVelocity(MyVec2(-1.0f, 0.0f));
        MyCircleShape shape;
        shape.setRadius(20.0f);
        body->createShape(&shape);
        body->setPosition(MyVec2(p.x, p.y));
        body->setAngularVelocity(0.05f);
        body->setLinearVelocity(MyVec2(5.0f, -5.0f));
        sprite = CCPhysicsSprite::create("Images/r2.png", CCRectMake(0,0,49,48));
        sprite->setScale(0.8);
        addChild(sprite);
    }
    m_counter++;
    body->setPosition(MyVec2(p.x, p.y));
    
    sprite->setMyBody(body);
    
    body->setUserData((void*)sprite);
}


void ContactSolverTestLayer::update(float dt)
{
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    //int velocityIterations = 8;
    int positionIterations = 3;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    m_pWorld->step(positionIterations);
    
}

void ContactSolverTestLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    //Add a new body/atlas sprite at the touched location
    CCSetIterator it;
    CCTouch* touch;
    
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
    
        if(!touch)
            break;
    
        CCPoint location = touch->getLocation();
    
        addNewSpriteAtPosition( location );
    }
}



void ContactSolverTestLayer::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}


///ContactListener
void MyContactListenerImpl2::BeginContact(MyContact* contact)
{
    if (contact)
    {
        MyBody* bodya = contact->getBodyA();
        MyBody* bodyb = contact->getBodyB();
        if (bodya->getShapeInfo()->getType() == bodyb->getShapeInfo()->getType())
        {
            bodya->setValid(false);
            bodyb->setValid(false);
        }
    }
}

void MyContactListenerImpl2::EndContact(MyContact* contact)
{
    MyBody* bodya = contact->getBodyA();
    MyBody* bodyb = contact->getBodyB();
    CCPhysicsSprite* spriteA = (CCPhysicsSprite*)bodya->getUserData();
    CCPhysicsSprite* spriteB = (CCPhysicsSprite*)bodyb->getUserData();
    if (spriteA)
    {
        //spriteA->setColor(ccc3(255, 255, 255));
    }
    if (spriteB)
    {
        //spriteB->setColor(ccc3(255, 255, 255));
    }
}