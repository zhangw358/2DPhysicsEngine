//
//  BroadphaseTestLayer.cpp
//  PhysicsProj
//
//  Created by 张唯 on 16/3/29.
//
//

#include "VisibleRect.h"
#include "cocos-ext.h"
#include "StartScene.h"
#include "BroadphaseTestLayer.h"

USING_NS_CC;
using extension::CCPhysicsSprite;

enum {
    kTagParentNode = 1,
};

BroadphaseTestLayer::BroadphaseTestLayer()
: m_pSpriteTexture(NULL)
, m_pWorld(NULL)
, m_counter(0)
, m_pListener(NULL)
{
    
}

BroadphaseTestLayer::~BroadphaseTestLayer()
{
    CC_SAFE_DELETE(m_pWorld);
    CC_SAFE_DELETE(m_pListener);
}

void BroadphaseTestLayer::onEnter()
{
    CCLayer::onEnter();
}

void BroadphaseTestLayer::initPhysics()
{
    m_pWorld = new MyWorld();
    m_pListener = new MyContactListenerImpl3();
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
bool BroadphaseTestLayer::init()
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
                                                              menu_selector(BroadphaseTestLayer::menuCloseCallback));
        
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
        
        for (int i = 1; i < 22; i++)
        {
            for (int j = 1; j < 13; j++)
            {
                addNewSpriteAtPosition(CCPoint(i * 43, j * 43));
            }
        }
        
        scheduleUpdate();
    }
    return bRet;
}


void BroadphaseTestLayer::createResetButton()
{
    CCMenuItemImage *reset = CCMenuItemImage::create("Images/r1.png", "Images/r2.png", this, menu_selector(BroadphaseTestLayer::reset));
    
    CCMenu *menu = CCMenu::create(reset, NULL);
    
    menu->setPosition(ccp(VisibleRect::bottom().x, VisibleRect::bottom().y + 30));
    this->addChild(menu, -1);
    
}

void BroadphaseTestLayer::reset(CCObject* sender)
{
    CCDirector::sharedDirector()->replaceScene(StartScene::create());
}

void BroadphaseTestLayer::draw()
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

void BroadphaseTestLayer::addNewSpriteAtPosition(CCPoint p)
{
    CCLOG("Add sprite %0.2f x %02.f",p.x,p.y);
    
    MyBody *body = m_pWorld->createBody();
    CCPhysicsSprite *sprite = NULL;
    MyCircleShape shape;
    shape.setRadius(12.0f);
    body->createShape(&shape);
    body->setPosition(MyVec2(p.x, p.y));
    body->setAngularVelocity(0.05f);
    if (m_counter > 250)
    {
        body->setLinearVelocity(MyVec2(2.0f, -1.0f));
    }
    m_counter++;
    sprite = CCPhysicsSprite::create("Images/r2.png", CCRectMake(0,0,49,48));
    sprite->setScale(0.5);
    addChild(sprite);
    body->setPosition(MyVec2(p.x, p.y));
    
    sprite->setMyBody(body);
    
    body->setUserData((void*)sprite);
}


void BroadphaseTestLayer::update(float dt)
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

void BroadphaseTestLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
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



void BroadphaseTestLayer::menuCloseCallback(CCObject* pSender)
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
void MyContactListenerImpl3::BeginContact(MyContact* contact)
{
    if (contact)
    {
        MyBody* bodya = contact->getBodyA();
        MyBody* bodyb = contact->getBodyB();
        //if (bodya->getShapeInfo()->getType() == bodyb->getShapeInfo()->getType())
        {
            //bodya->setValid(false);
            //bodyb->setValid(false);
        }
    }
}

void MyContactListenerImpl3::EndContact(MyContact* contact)
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
