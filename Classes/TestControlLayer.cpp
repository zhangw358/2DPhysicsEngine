//
//  TestControlLayer.cpp
//  PhysicsProj
//
//  Created by 张唯 on 16/3/28.
//
//

#include "TestControlLayer.h"
#include "VisibleRect.h"
#include "resourcePath.h"
#include "ContactSolverTestLayer.h"
#include "NarrowphaseTestLayer.h"
#include "BroadphaseTestLayer.h"
#include "GameLayer.h"
#include "StartScene.h"

TestControlLayer::TestControlLayer()
: m_pItemMenu(NULL)
{
    
}

TestControlLayer::~TestControlLayer()
{
    
}


bool TestControlLayer::init()
{
    bool bRet = CCLayer::init();
    if (bRet)
    {
        //CCSprite * bg = CCSprite::create("bg.png");
        //CCSize size = bg->getContentSize();
        //bg->setPosition(VisibleRect::center());
        //addChild(bg);
        
        // add close menu
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(s_pPathClose, s_pPathClose, this, menu_selector(TestControlLayer::closeCallback) );
        CCMenu* pMenu =CCMenu::create(pCloseItem, NULL);
        
        pMenu->setPosition( CCPointZero );
        pCloseItem->setPosition(ccp( VisibleRect::right().x - 30, VisibleRect::top().y - 30));
        
        CCLabelTTF* label = CCLabelTTF::create("MyPhysicsEngine", "Arial", 55);
        label->setPosition(ccp(VisibleRect::center().x, VisibleRect::top().y - 60));
        addChild(label);
        
        // add menu items for tests
        m_pItemMenu = CCMenu::create();
        for (int i = 0; i < TESTS_COUNT; ++i)
        {
            CCLabelTTF* label = CCLabelTTF::create(g_aTestNames[i].c_str(), "Arial", 55);
            CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(TestControlLayer::menuCallback));
            
            m_pItemMenu->addChild(pMenuItem, i + 10000);
            pMenuItem->setPosition( ccp( VisibleRect::center().x, (VisibleRect::top().y - 120 - (i + 1) * LINE_SPACE) ));
        }
        
        m_pItemMenu->setContentSize(CCSizeMake(VisibleRect::getVisibleRect().size.width, (TESTS_COUNT + 1) * (LINE_SPACE)));
        m_pItemMenu->setPosition(CCPointZero);
        addChild(m_pItemMenu);
        
        setTouchEnabled(true);
        addChild(pMenu, 1);
        
    }
    return bRet;
}



void TestControlLayer::closeCallback(cocos2d::CCObject *pSender)
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

void TestControlLayer::menuCallback(cocos2d::CCObject *pSender)
{
    // get the userdata, it's the index of the menu item clicked
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int nIdx = pMenuItem->getZOrder() - 10000;
    
    // create the test scene and run it
    CCLayer* pLayer = createTestLayer(nIdx);
    if (pLayer)
    {
        CCScene* pScene = CCDirector::sharedDirector()->getRunningScene();
        ((StartScene*)pScene)->replaceLayer(pLayer);
    }
}

CCLayer* TestControlLayer::createTestLayer(int index)
{
    switch (index) {
        case TEST_CONTACT_SOLVER:
            return ContactSolverTestLayer::create();
        case TEST_BROADPHASE:
            return BroadphaseTestLayer::create();
        case TEST_NARROWPHASE:
            return NarrowphaseTestLayer::create();
        case TEST_DEMO:
            return GameLayer::create();
    }
}