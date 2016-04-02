//
//  MyWorld.cpp
//  PhysicsProj
//
//  Created by 张唯 on 16/3/16.
//
//

#include "MyWorld.h"
#include "MyBody.h"
#include "VisibleRect.h"
#include "QuadTree.h"

MyWorld::MyWorld()
: m_contactManager()
{
    
}

MyWorld::~MyWorld()
{
    int nBodyCount = (int)m_vecBody.size();
    for (int i = 0; i < nBodyCount; i ++)
    {
        MyBody* body = m_vecBody.at(i);
        if (body)
        {
            delete body;
        }
    }
}

MyBody* MyWorld::createBody()
{
    MyBody* newBody = new MyBody();
    m_vecBody.push_back(newBody);
    return newBody;
}

void MyWorld::step(int nPositionIter)
{
    // 1: ContactManager checkContact, notice listener
    m_contactManager.checkContact();
    
    // 2: check body status, some body are erased in contact callback;
    checkBodyValid();
    
    // 3: solve contact;
    m_contactManager.solveContact(nPositionIter);
    
    // 4: update pos/velocity
    updateBodiesPos();
    
    // 5: ContactManger findNewContact
    m_contactManager.findNewContact(m_vecBody);
}

void MyWorld::setContactListener(MyContactListener *p)
{
    m_contactManager.setContactListener(p);
}

void MyWorld::checkBodyValid()
{
    vector<MyBody*>::iterator it = m_vecBody.begin();
    for (; it != m_vecBody.end(); )
    {
        MyBody* body = *it;
        if (!body->getValid())
        {
            CCSprite* sprite = (CCSprite*)body->getUserData();
            sprite->removeFromParent();
            it = m_vecBody.erase(it);
            delete body;
        }
        else
        {
            it++;
        }
    }
}

MyBody* MyWorld::getTouchingBody(int x, int y)
{
    MyBody* body = NULL;
    vector<MyBody*>::iterator it = m_vecBody.begin();
    for (; it != m_vecBody.end(); it++)
    {
        MyAABB rect = (*it)->getAABBInfo();
        if (rect.bContains(x, y))
        {
            body = *it;
        }
    }
    return body;
}

void MyWorld::updateBodiesPos()
{
    int nBodyCount = (int)m_vecBody.size();
    for (int i = 0; i < nBodyCount; i ++)
    {
        MyBody* body = m_vecBody.at(i);
        if (body)
        {
            body->update();
        }
    }
}