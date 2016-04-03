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
#include "MinipoolBall.h"

USING_NS_CC;
using extension::CCPhysicsSprite;

#define LENGTH 145
#define HEIGHT 200

#define SHOTPOWER 8.0f

GameLayer::GameLayer()
: m_pWorld(NULL)
, m_pListener(NULL)
, m_gameBatchNode(NULL)
, m_playerBall(NULL)
, m_cue(NULL)
, m_touch(NULL)
, m_pullBack(0)
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");
    m_gameBatchNode = CCSpriteBatchNode::create("sprite_sheet.png", 50);
    this->addChild(m_gameBatchNode, 1);
    
    m_lineContainer = LineContainer::create();
    this->addChild(m_lineContainer);
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
    
    CCPoint center = VisibleRect::center();
    for (int i = 0; i < 6; i++)
    {
        MyBody *body = m_pWorld->createBody();
        MyCircleShape shape;
        shape.setRadius(2 * BALL_RADIUS);
        body->createShape(&shape);
        body->setGroupData(101);
        //CCPhysicsSprite* sprite = CCPhysicsSprite::create("Images/r2.png", CCRectMake(0,0,49,48));
        //addChild(sprite, 100);
        if (i < 3)
        {
            body->setPosition(MyVec2(center.x - LENGTH, center.y + (i-1) * HEIGHT));
        }
        else
        {
            body->setPosition(MyVec2(center.x + LENGTH, center.y + (i-4) * HEIGHT));
        }
    }
    
    // top
    MyBody *body = m_pWorld->createBody();
    MyEdgeShape shape1;
    shape1.setPosition(MyVec2(372, 512), MyVec2(590, 512));
    body->createShape(&shape1);
    
    // bottom
    MyBody *body2 = m_pWorld->createBody();
    MyEdgeShape shape2;
    shape2.setPosition(MyVec2(372, 122), MyVec2(590, 122));
    body2->createShape(&shape2);
    
    // left
    MyBody *body3 = m_pWorld->createBody();
    MyEdgeShape shape3;
    shape3.setPosition(MyVec2(354,138), MyVec2(354,295));
    body3->createShape(&shape3);
    
    MyBody *body4 = m_pWorld->createBody();
    MyEdgeShape shape4;
    shape4.setPosition(MyVec2(354,340), MyVec2(354,494));
    body4->createShape(&shape4);
    
    // right
    MyBody *body5 = m_pWorld->createBody();
    MyEdgeShape shape5;
    shape5.setPosition(MyVec2(610, 138), MyVec2(610, 295));
    body5->createShape(&shape5);
    
    MyBody *body6 = m_pWorld->createBody();
    MyEdgeShape shape6;
    shape6.setPosition(MyVec2(610, 340), MyVec2(610, 494));
    body6->createShape(&shape6);
    
    MinipoolBall *ball;
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    int col = 5;
    int colCnt = 0;
    int startX;
    int startY;
    startX = (screenSize.width/2 - 8 * BALL_RADIUS);
    startY = screenSize.height * 0.82f;
    float newX;
    float newY;
    int color;
    for (int i = 0; i < 15; i++)
    {
        if (i == 10) {
            color = enColorBlack;
        } else if (i % 2 == 0) {
            color = enColorYellow;
        } else {
            color = enColorRed;
        }
        if (colCnt < col) {
            newX = startX + colCnt * 4 * BALL_RADIUS;
            colCnt++;
        } else {
            startX += 2 * BALL_RADIUS;
            startY -= 4 * BALL_RADIUS;
            newX = startX;
            col--;
            colCnt = 1;
        }
        newY = startY;
        ball = MinipoolBall::create(this, ccp(newX, newY-30), color);
        m_gameBatchNode->addChild(ball, 1);
    }
    
    
    // add player ball
    m_playerBall = MinipoolBall::create(this, ccp(screenSize.width*0.5f, screenSize.height*0.3f), enColorWhite);
    m_playerBall->getMyBody()->setGroupData(51);
    m_gameBatchNode->addChild(m_playerBall, 1);
    
    // add cue
    m_cue = MinipoolCue::create(this);
    m_cue->getMyBody()->setPosition(MyVec2(screenSize.width*0.2f, screenSize.height* 0.5f));
    m_cue->getMyBody()->setAngle(acos(-1)/2);
    m_gameBatchNode->addChild(m_cue, 1);
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
        
        
        // create reset button
        this->createResetButton();
        
        CCSprite * bg = CCSprite::create("bg.png");
        bg->setPosition(VisibleRect::center());
        this->addChild(bg);
        
        // init physics
        this->initPhysics();
        
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

void GameLayer::ccTouchesBegan(CCSet *touches, CCEvent* event)
{
    CCTouch *touch = (CCTouch*)touches->anyObject();
    if (touch)
    {
        CCPoint playerPos = m_playerBall->getPosition();
        CCPoint tap = touch->getLocation();

        float diffx = tap.x - playerPos.x;
        float diffy = tap.y - playerPos.y;
        float diff = pow(diffx, 2) + pow(diffy, 2);
        if(diff < pow(BALL_RADIUS * 4, 2))
        {
            if(diff > pow(BALL_RADIUS * 2, 2))
            {
                float angle = atan2(diffy,diffx);
                m_cueStartPoint = ccp(playerPos.x + BALL_RADIUS * 0.8f * cos(angle),
                                     playerPos.y + BALL_RADIUS * 0.8f * sin(angle));
            }else{
                m_cueStartPoint = playerPos;
            }
            m_lineContainer->setBallPoint(m_cueStartPoint);
            m_touch = touch;
        }
    }
}

void GameLayer::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
    CCTouch *touch = (CCTouch*)touches->anyObject();
    if(touch && touch == m_touch)
    {
        CCPoint tap = touch->getLocation();
        float diffx = tap.x - m_playerBall->getPosition().x;
        float diffy = tap.y - m_playerBall->getPosition().y;
        
        if(pow(diffx, 2) + pow(diffy, 2) > pow(BALL_RADIUS * 2, 2)){

            m_cue->setVisible(true);
            m_lineContainer->setDrawing(true);
            placeCue(tap);
        }else{
            m_lineContainer->setDrawing(false);
            m_cue->setVisible(false);

        }
    }
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
        CCLog("touch: %f,%f" ,location.x, location.y);
    }
    
    if (m_touch)
    {
        float angle = m_cue->getMyBody()->getTransformInfo().q.getAngle();
        float power = m_pullBack / 100.0f;
        m_playerBall->getMyBody()->setLinearVelocity(MyVec2(power * cos(angle) * SHOTPOWER, power * sin(angle) * SHOTPOWER));
    }
    m_touch = NULL;
    m_lineContainer->setDrawing(false);
    m_cue->setVisible(false);
}

void GameLayer::placeCue(CCPoint pos)
{
    float diffx = m_cueStartPoint.x - pos.x;
    float diffy = m_cueStartPoint.y - pos.y;
    
    float angle = atan2(diffy, diffx);
    float distance = sqrt(pow (diffx, 2) + pow(diffy, 2));
    
    m_pullBack = distance * 0.5f;
    CCPoint cuePosition = ccp(
                              m_cueStartPoint.x - (BALL_RADIUS * 21 + m_pullBack) * cos(angle),
                              m_cueStartPoint.y - (BALL_RADIUS * 21 + m_pullBack) * sin(angle));
    
    m_cue->getMyBody()->setPosition(MyVec2(cuePosition.x, cuePosition.y));
    m_cue->getMyBody()->setAngle(angle);
    m_lineContainer->setCuePoint(ccp(
                                    m_cueStartPoint.x - ( m_pullBack) * cos(angle),
                                    m_cueStartPoint.y - ( m_pullBack) * sin(angle)));
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
        if (bodya->getGroupData() + bodyb->getGroupData() > 150)
        {
            if (bodya->getGroupData() == 101)
            {
                if (bodyb->getGroupData() == 51)
                {
                    MinipoolBall* ball = (MinipoolBall*)(bodyb->getUserData());
                    ball->reset();
                }
                else
                {
                    bodyb->setValid(false);
                }
            }
            else
            {
                if (bodya->getGroupData() == 51)
                {
                    MinipoolBall* ball = (MinipoolBall*)(bodya->getUserData());
                    ball->reset();
                }
                else
                {
                    bodya->setValid(false);
                }
            }
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
