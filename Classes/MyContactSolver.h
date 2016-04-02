//
//  MyContactSolver.h
//  PhysicsProj
//
//  Created by 张唯 on 16/3/27.
//
//

#ifndef MyContactSolver_h
#define MyContactSolver_h

#include "MyContact.h"
#include <list>

using std::list;

class MyContactSolver
{
public:
    MyContactSolver();
    ~MyContactSolver();
    void solve(list<MyContact*>& l);
    void setPositionIter(int positionIter);
    
private:
    void solvePositionConstrain(MyContact* contact);
    void solveEdgeContact(MyContact* contact);
    void solveNormalContact(MyContact* contact);
    bool TestOverlap(MyContact* c);
    
private:
    int m_positionIter;
};

#endif /* MyContactSolver_h */
