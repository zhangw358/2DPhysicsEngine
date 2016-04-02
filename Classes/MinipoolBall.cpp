//
//  MinipoolBall.cpp
//  PhysicsProj
//
//  Created by 张唯 on 16/4/2.
//
//

#include "MinipoolBall.h"
#include "MyPhysics.h"
#include "GameLayer.h"

MinipoolBall::~MinipoolBall()
{
    
}

MinipoolBall::MinipoolBall(CCLayer *game, CCPoint position, int color)
: m_pGameLayer(game)
, m_startPosition(position)
, m_color(color)
, m_bInPlay(true)
{
    
}

MinipoolBall* MinipoolBall::create(CCLayer *game, CCPoint position, int color)
{
    MinipoolBall *sprite = new MinipoolBall(game, position, color);
    if(sprite)
    {
        sprite->initBall();
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void MinipoolBall::initBall()
{
    MyBody *body = ((GameLayer*)m_pGameLayer)->getWorld()->createBody();
    MyCircleShape shape;
    shape.setRadius(BALL_RADIUS);
    body->createShape(&shape);
    body->setPosition(MyVec2(m_startPosition.x, m_startPosition.y));
    body->setAngularVelocity(0.05f);
    body->setUserData((void*)this);
    setMyBody(body);
    
    switch(m_color){
        case enColorBlack:
            this->initWithSpriteFrameName("ball_black.png");
            break;
        case enColorRed:
            this->initWithSpriteFrameName("ball_red.png");
            break;
        case enColorYellow:
            this->initWithSpriteFrameName("ball_yellow.png");
            break;
        case enColorWhite:
            this->initWithSpriteFrameName("ball_white.png");
            break;
    }
    
}

void MinipoolBall::reset()
{
    MyBody* body = getMyBody();
    if (body)
    {
        body->setLinearVelocity(MyVec2(0,0));
        body->setAngularVelocity(0);
        body->setPosition(MyVec2(m_startPosition.x, m_startPosition.y));
    }
}