#include "StartScene.h"
#include "VisibleRect.h"
#include "NarrowphaseTestLayer.h"
#include "cocos-ext.h"
#define PTM_RATIO 32

USING_NS_CC;
using extension::CCPhysicsSprite;

enum {
    kTagParentNode = 1,
};

NarrowphaseTestLayer::NarrowphaseTestLayer()
: m_pSpriteTexture(NULL)
, m_pWorld(NULL)
, m_counter(0)
, m_pListener(NULL)
, m_pHoldingBody(NULL)
{
    
}

NarrowphaseTestLayer::~NarrowphaseTestLayer()
{
    CC_SAFE_DELETE(m_pWorld);
    CC_SAFE_DELETE(m_pListener);
}

void NarrowphaseTestLayer::onEnter()
{
    CCLayer::onEnter();
}

void NarrowphaseTestLayer::initPhysics()
{
    m_pWorld = new MyWorld();
    m_pListener = new MyContactListenerImpl();
    m_pWorld->setContactListener(m_pListener);
    /*
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    world = new b2World(gravity);
    
    // Do we want to let bodies sleep?
    //world->SetAllowSleeping(true);
    
    world->SetContinuousPhysics(true);
    world->SetContactListener(m_pListener);
    //     m_debugDraw = new GLESDebugDraw( PTM_RATIO );
    //     world->SetDebugDraw(m_debugDraw);
    
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    //        flags += b2Draw::e_jointBit;
    //        flags += b2Draw::e_aabbBit;
    //        flags += b2Draw::e_pairBit;
    //        flags += b2Draw::e_centerOfMassBit;
    //m_debugDraw->SetFlags(flags);
    
    
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner
    
    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    */
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
bool NarrowphaseTestLayer::init()
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
                                                              menu_selector(NarrowphaseTestLayer::menuCloseCallback));
        
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
        
        addNewSpriteAtPosition(CCPoint(VisibleRect::center().x-200, VisibleRect::center().y+200));
        addNewSpriteAtPosition(CCPoint(VisibleRect::center().x+200, VisibleRect::center().y+200));
        addNewSpriteAtPosition(CCPoint(VisibleRect::center().x-200, VisibleRect::center().y-200));
        addNewSpriteAtPosition(CCPoint(VisibleRect::center().x+200, VisibleRect::center().y-200));
        addNewSpriteAtPosition2(VisibleRect::center());
        
        scheduleUpdate();
    }
    return bRet;
}


void NarrowphaseTestLayer::createResetButton()
{
    CCMenuItemImage *reset = CCMenuItemImage::create("Images/r1.png", "Images/r2.png", this, menu_selector(NarrowphaseTestLayer::reset));
    
    CCMenu *menu = CCMenu::create(reset, NULL);
    
    menu->setPosition(ccp(VisibleRect::bottom().x, VisibleRect::bottom().y + 30));
    this->addChild(menu, -1);
    
}

void NarrowphaseTestLayer::reset(CCObject* sender)
{
    CCDirector::sharedDirector()->replaceScene(StartScene::create());
}

void NarrowphaseTestLayer::draw()
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

void NarrowphaseTestLayer::addNewSpriteAtPosition2(CCPoint p)
{
    MyBody *body = m_pWorld->createBody();
    MyCircleShape shape;
    shape.setRadius(24.0f);
    body->createShape(&shape);
    body->setPosition(MyVec2(p.x, p.y));
    body->setAngularVelocity(0.05f);
    body->setLinearVelocity(MyVec2(1.5f, 1.5f));
    CCPhysicsSprite *sprite = CCPhysicsSprite::create("Images/r2.png", CCRectMake(0,0,48,48));
    body->setUserData((void*)sprite);
    sprite->setMyBody(body);
    addChild(sprite);
}

void NarrowphaseTestLayer::addNewSpriteAtPosition(CCPoint p)
{
    CCLOG("Add sprite %0.2f x %02.f",p.x,p.y);
    
    MyBody *body = m_pWorld->createBody();
    MyPolygonShape shape;
    shape.setAsBox(16, 160);
    body->createShape(&shape);
    body->setPosition(MyVec2(p.x, p.y));
    body->setAngularVelocity(0.01f);
    body->setSensor(true);
    //We have a 64x64 sprite sheet with 4 different 32x32 images.  The following code is
    //just randomly picking one of the images
    CCPhysicsSprite *sprite = CCPhysicsSprite::createWithTexture(m_pSpriteTexture,CCRectMake(32,32,32,320));
    CCNode *parent = this->getChildByTag(kTagParentNode);
    parent->addChild(sprite);
    body->setPosition(MyVec2(p.x, p.y));
    sprite->setMyBody(body);
    body->setUserData((void*)sprite);
}


void NarrowphaseTestLayer::update(float dt)
{
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    //int velocityIterations = 8;
    //int positionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    int positionIterations = 1;
    m_pWorld->step(positionIterations);
    
    /*
    std::vector<b2Body*>& vec = m_pListener->getDestroyBodyList();
    for (int i = 0; i < vec.size(); i++)
    {
        b2Body *body = vec.at(i);
        if (body)
        {
            CCSprite* sp = (CCSprite*) body->GetUserData();
            world->DestroyBody(body);
            body = NULL;
            if (sp != NULL)
            {
                sp->removeFromParent();
            }
        }
    }
    m_pListener->resetDestoryBodyList();
    */
}

void NarrowphaseTestLayer::ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    CCSetIterator it;
    CCTouch* touch;
    
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
    
        if(!touch)
            break;
    
        CCPoint p = touch->getLocation();
        MyBody* body = m_pWorld->getTouchingBody(p.x, p.y);
        if (body != NULL)
        {
            m_pHoldingBody = body;
        }
    }
}

void NarrowphaseTestLayer::ccTouchesMoved(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    CCSetIterator it;
    CCTouch* touch;
    
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint p = touch->getLocation();
        if (m_pHoldingBody)
        {
            m_pHoldingBody->setPosition(MyVec2(p.x, p.y)) ;
        }
    }
    
}

void NarrowphaseTestLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    m_pHoldingBody = NULL;
    //Add a new body/atlas sprite at the touched location
    //CCSetIterator it;
    //CCTouch* touch;
    
    //for( it = touches->begin(); it != touches->end(); it++)
    //{
        //touch = (CCTouch*)(*it);
        
        //if(!touch)
        //    break;
        
        //CCPoint location = touch->getLocation();
        
        //addNewSpriteAtPosition( location );
    //}
}



void NarrowphaseTestLayer::menuCloseCallback(CCObject* pSender)
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
void MyContactListenerImpl::BeginContact(MyContact* contact)
{
   if (contact)
   {
       MyBody* bodya = contact->getBodyA();
       MyBody* bodyb = contact->getBodyB();
       if (bodya->getShapeInfo()->getType() == MyShape::e_shape_edge)
       {
           //bodyb->setValid(false);
       }
       
       if (bodyb->getShapeInfo()->getType() == MyShape::e_shape_edge)
       {
           //bodya->setValid(false);
       }
       
       CCPhysicsSprite* spriteA = (CCPhysicsSprite*)bodya->getUserData();
       CCPhysicsSprite* spriteB = (CCPhysicsSprite*)bodyb->getUserData();
       if (spriteA)
       {
           spriteA->setColor(ccc3(255, 0, 0));
       }
       if (spriteB)
       {
           spriteB->setColor(ccc3(255, 0, 0));
       }
       //bodya->setAngularVelocity(-bodya->getAngularVelocity());
       //bodyb->setAngularVelocity(-bodyb->getAngularVelocity());
   }
}

void MyContactListenerImpl::EndContact(MyContact* contact)
{
    MyBody* bodya = contact->getBodyA();
    MyBody* bodyb = contact->getBodyB();
    CCPhysicsSprite* spriteA = (CCPhysicsSprite*)bodya->getUserData();
    CCPhysicsSprite* spriteB = (CCPhysicsSprite*)bodyb->getUserData();
    if (spriteA)
    {
        spriteA->setColor(ccc3(255, 255, 255));
    }
    if (spriteB)
    {
        spriteB->setColor(ccc3(255, 255, 255));
    }
}