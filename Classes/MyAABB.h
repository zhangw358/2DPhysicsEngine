//
//  MyAABB.h
//  PhysicsProj
//
//  Created by 张唯 on 16/3/21.
//
//

#ifndef MyAABB_h
#define MyAABB_h


#include "MyMathDef.h"

class MyAABB
{
public:
    MyVec2 getLowerBound() const {return lowerBound;}
    MyVec2 getUpperBound() const {return upperBound;}
    
    MyAABB ()
    {
        lowerBound.setZero();
        upperBound.setZero();
    }
    
    MyAABB (MyVec2 l, MyVec2 u)
    {
        lowerBound = l;
        upperBound = u;
    }
    
    bool bContains(int x, int y)
    {
        int lx = x - lowerBound.x;
        int ly = y - lowerBound.y;
        int ux = upperBound.x - x;
        int uy = upperBound.y - y;
        
        if (lx > 0 && ly > 0 && ux > 0&& uy > 0)
        {
            return true;
        }
        
        return false;
    }
    
private:
    MyVec2 lowerBound;
    MyVec2 upperBound;
};

#endif /* MyAABB_h */
