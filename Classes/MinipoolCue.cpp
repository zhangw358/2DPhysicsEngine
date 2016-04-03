//
//  MinipoolCue.cpp
//  PhysicsProj
//
//  Created by 张唯 on 16/4/3.
//
//

#include "MinipoolCue.h"
#include "GameLayer.h"

#define BALL_RADIUS 6.0

MinipoolCue::~MinipoolCue()
{
    
}

MinipoolCue::MinipoolCue(CCLayer *game)
: m_pLayer(game)
{
    
}

MinipoolCue* MinipoolCue::create(CCLayer *game)
{
    MinipoolCue *sprite = new MinipoolCue(game);
    if (sprite)
    {
        sprite->initCue();
        sprite->autorelease();
    }
    else
    {
        CC_SAFE_RELEASE(sprite);
    }
    return sprite;
}

void MinipoolCue::initCue()
{
    MyBody *body = ((GameLayer*)m_pLayer)->getWorld()->createBody();
    MyPolygonShape shape;
    shape.setAsBox(BALL_RADIUS * 21, BALL_RADIUS * 0.2);
    body->createShape(&shape);
    body->setUserData((void*)this);
    body->setSensor(true);
    setMyBody(body);
    this->initWithSpriteFrameName("cue.png");
}