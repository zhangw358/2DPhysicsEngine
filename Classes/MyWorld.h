//
//  MyWorld.hpp
//  PhysicsProj
//
//  Created by 张唯 on 16/3/16.
//
//

#ifndef MyWorld_h
#define MyWorld_h

#include <vector>
#include <map>
#include "MyContactManager.h"
using std::vector;

class MyBody;

class MyWorld
{
public:
    MyWorld();
    ~MyWorld();
    void step(int nPositionIter);
    MyBody* createBody();
    
    void setContactListener(MyContactListener* p);
    MyBody* getTouchingBody(int x, int y);
private:
    void updateBodiesPos();
    void checkBodyValid();
    MyContactManager m_contactManager;
    vector<MyBody*> m_vecBody;
};

#endif /* MyWorld_hpp */
