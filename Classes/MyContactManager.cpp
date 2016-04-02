//
//  MyContactManager.cpp
//  PhysicsProj
//
//  Created by 张唯 on 16/3/22.
//
//

#include "MyContactManager.h"
#include "GJKDistance.h"
#include <algorithm>

using std::sort;

MyContactManager::MyContactManager()
: m_quadTree()
, m_pContactListener(NULL)
, m_contactSolver()
{
}

MyContactManager::~MyContactManager()
{
}

void MyContactManager::addPhysicBody(MyBody *body)
{
    m_quadTree.addUserData(body, body->getAABBInfo());
}

void MyContactManager::clearPhysicData()
{
    m_quadTree.clearTree();
}

void MyContactManager::checkContact()
{
    removeDuplicateContact();
    broadcastContact();
    updateContact();
}

bool MyContactManager::TestOverlap(MyContact* c)
{
    MyShape* shapeA = c->getBodyA()->getShapeInfo();
    MyShape* shapeB = c->getBodyB()->getShapeInfo();
    MyTransform xfA = c->getBodyA()->getTransformInfo();
    MyTransform xfB = c->getBodyB()->getTransformInfo();
    
    GJKDistanceInput input;
    input.proxyA.set(shapeA);
    input.proxyB.set(shapeB);
    input.transformA = xfA;
    input.transformB = xfB;
    input.useRadii = true;
    
    GJKSimplexCache cache;
    cache.count = 0;
    
    GJKDistanceOutput output;
    
    GJKDistance(&output, &cache, &input);
    
    return output.distance < 10.0f * FLT_EPSILON;
}

void MyContactManager::broadPhase(vector<MyBody*>& vecBody)
{
    clearPhysicData();
    int nSize = (int)vecBody.size();
    for (int i = 0; i < nSize; i++)
    {
        addPhysicBody(vecBody.at(i));
    }
    m_quadTree.checkContact();
}

void MyContactManager::narrowPhase()
{
    list<MyContact*>& l = m_quadTree.getAABBContactList();
    
    list<MyContact*>::iterator it = l.begin();
    for (; it != l.end(); it ++)
    {
        bool bGJKTest = TestOverlap(*it);
        if (bGJKTest)
        {
            m_contactList.push_back(*it);
        }
    }
    
}

void MyContactManager::findNewContact(vector<MyBody*>& vecBody)
{
    broadPhase(vecBody);
    narrowPhase();
}

void MyContactManager::setContactListener(MyContactListener *p)
{
    m_pContactListener = p;
}


void MyContactManager::removeDuplicateContact()
{
    m_contactList.sort();
    list<MyContact*>::iterator it = m_contactList.begin();
    
    // remove duplicate contact;
    for (; it != m_contactList.end(); it++)
    {
        while (true)
        {
            list<MyContact*>::iterator it2 = it;
            it2++;
            if (it2 == m_contactList.end())
            {
                break;
            }
            
            if ((*it2)->getBodyA() != (*it)->getBodyA() || (*it2)->getBodyB() != (*it)->getBodyB())
            {
                break;
            }
            else
            {
                it = m_contactList.erase(it);
            }
        }
    }
 
}

void MyContactManager::broadcastContact()
{
    list<MyContact*>::iterator it = m_contactList.begin();
    for (; it != m_contactList.end() ;)
    {
        MyContact* pContact = (*it);
        bool bGJKTest = TestOverlap(pContact);
        if (!bGJKTest)
        {
            if (m_pContactListener)
            {
                m_pContactListener->EndContact(pContact);
            }
            it = m_contactList.erase(it);
        }
        else
        {
            if (m_pContactListener)
            {
                m_pContactListener->BeginContact(pContact);
            }
            it++;
        }
    }

}

void MyContactManager::updateContact()
{
    list<MyContact*>::iterator it = m_contactList.begin();
    for (; it != m_contactList.end() ;)
    {
        MyContact* pContact = (*it);
        
        if (pContact->getBodyA()->getValid() && pContact->getBodyB()->getValid())
        {
            it++;
        }
        else
        {
            it = m_contactList.erase(it);
        }
    }
}

void MyContactManager::solveContact(int nPositionIter)
{
    m_contactSolver.setPositionIter(nPositionIter);
    m_contactSolver.solve(m_contactList);
}