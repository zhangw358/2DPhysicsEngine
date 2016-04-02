#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"

using cocos2d::CCLayer;

class StartScene : public cocos2d::CCScene
{
public:
    CREATE_FUNC(StartScene);
    StartScene();
    ~StartScene();
    void replaceLayer(CCLayer* pLayer);
protected:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
private:
    CCLayer* m_pLayer;
    
};

#endif // __START_SCENE_H__
