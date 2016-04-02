//
//  GameLayer.cpp
//  PhysicsProj
//
//  Created by 张唯 on 16/4/2.
//
//

#include "VisibleRect.h"
#include "cocos-ext.h"
#include "StartScene.h"
#include "GameLayer.h"

USING_NS_CC;
using extension::CCPhysicsSprite;

GameLayer::GameLayer()
: m_pWorld(NULL)
, m_pListener(NULL)
{
    
}

GameLayer::~GameLayer()
{
    CC_SAFE_DELETE(m_pWorld);
    CC_SAFE_DELETE(m_pListener);
}

void GameLayer::onEnter()
{
    CCLayer::onEnter();
}

void GameLayer::initPhysics()
{
    m_pWorld = new MyWorld();
    m_pListener = new MyContactListenerImpl4();
    m_pWorld->setContactListener(m_pListener);
    
    /*
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
     */
}


// on "init" you need to initialize your instance
bool GameLayer::init()
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
                                                              menu_selector(GameLayer::menuCloseCallback));
        
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
        
        CCSprite * bg = CCSprite::create("bg.png");
        bg->setPosition(VisibleRect::center());
        this->addChild(bg);
        
        scheduleUpdate();
    }
    return bRet;
}


void GameLayer::createResetButton()
{
    CCMenuItemImage *reset = CCMenuItemImage::create("Images/r1.png", "Images/r2.png", this, menu_selector(GameLayer::reset));
    
    CCMenu *menu = CCMenu::create(reset, NULL);
    
    menu->setPosition(ccp(VisibleRect::bottom().x, VisibleRect::bottom().y + 30));
    this->addChild(menu, -1);
    
}

void GameLayer::reset(CCObject* sender)
{
    CCDirector::sharedDirector()->replaceScene(StartScene::create());
}

void GameLayer::draw()
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

void GameLayer::update(float dt)
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

void GameLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
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
    }
}



void GameLayer::menuCloseCallback(CCObject* pSender)
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
void MyContactListenerImpl4::BeginContact(MyContact* contact)
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

void MyContactListenerImpl4::EndContact(MyContact* contact)
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
