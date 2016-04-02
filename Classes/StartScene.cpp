#include "StartScene.h"
#include "ContactSolverTestLayer.h"
#include "TestControlLayer.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool StartScene::init()
{
    //////////////////////////////
    // 1. super init first
    bool bRet = CCScene::init();
    if (bRet)
    {
        //StartLayer *layer = StartLayer::create();
        //ContactSolverTestLayer *layer = ContactSolverTestLayer::create();
        TestControlLayer* layer = TestControlLayer::create();
        replaceLayer(layer);
    }
    return bRet;
}

StartScene::StartScene()
: m_pLayer(NULL)
{
}

StartScene::~StartScene()
{
}

void StartScene::onEnter()
{
    CCScene::onEnter();
}

void StartScene::onExit()
{
    CCScene::onExit();
}

void StartScene::replaceLayer(CCLayer* pLayer)
{
    if (m_pLayer)
    {
        removeChild(m_pLayer);
    }
    addChild(pLayer);
    m_pLayer = pLayer;
}