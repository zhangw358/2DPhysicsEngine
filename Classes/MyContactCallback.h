//
//  MyContactListener.h
//  PhysicsProj
//
//  Created by 张唯 on 16/3/22.
//
//

#ifndef MyContactCallback_h
#define MyContactCallback_h

#include "MyContact.h"

class MyContactListener
{
public:
    virtual ~MyContactListener() {}
    
    /// Called when two fixtures begin to touch.
    virtual void BeginContact(MyContact* contact) {}
    
    /// Called when two fixtures cease to touch.
    virtual void EndContact(MyContact* contact) {}
    
    
    //virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) { }
    
    //virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) { }
};
#endif /* MyContactListener_h */
