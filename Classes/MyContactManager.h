//
//  MyContactManager.h
//  PhysicsProj
//
//  Created by 张唯 on 16/3/22.
//
//

#ifndef MyContactManager_h
#define MyContactManager_h

#include "QuadTree.h"
#include "MyContactCallback.h"
#include "MyContactSolver.h"
#include <list>

using std::vector;
using std::list;

class MyContactManager
{
public:
    MyContactManager();
    ~MyContactManager();
    void checkContact();
    
    void findNewContact(vector<MyBody*>& vecBody);
    void addPhysicBody(MyBody* body);
    void clearPhysicData();
    
    void setContactListener(MyContactListener* p);
    
    void solveContact(int nPositionIter);
private:
    void broadPhase(vector<MyBody*>& vecBody);
    void narrowPhase();
    bool TestOverlap(MyContact* c);
    void removeDuplicateContact();
    void broadcastContact();
    void updateContact();
private:
    list<MyContact*> m_contactList;
    MyContactListener *m_pContactListener;
    QuadTree m_quadTree;
    MyContactSolver m_contactSolver;
};

#endif /* MyContactManager_h */
