//
//  LineContainer.cpp
//  PhysicsProj
//
//  Created by 张唯 on 16/4/3.
//
//

#include "LineContainer.h"

USING_NS_CC;

LineContainer::LineContainer()
{
    m_dash = 5;
    m_dashSpace = 5;
    m_screenSize = CCDirector::sharedDirector()->getWinSize();
    m_drawing = false;
    ccPointSize(0.2f);
}

void LineContainer::draw()
{
    if(m_drawing)
    {
        ccDrawColor4F(1.0,1.0,1.0,1.0);
        
        float length = ccpDistance(m_ballPoint,m_cuePoint);
        int segments = length / (m_dash + m_dashSpace);
        
        float t = 0.0f;
        float x_;
        float y_;
        
        for (int i = 0; i < segments + 1; ++i)
        {
            x_ = m_cuePoint.x + t * (m_ballPoint.x - m_cuePoint.x);
            y_ = m_cuePoint.y + t * (m_ballPoint.y - m_cuePoint.y);
            
            ccDrawCircle(ccp(x_,y_),3,M_PI,4,false);
            
            t += (float)1 / segments;
        }
    }
}
