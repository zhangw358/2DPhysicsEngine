//
//  MyContactSolver.cpp
//  PhysicsProj
//
//  Created by 张唯 on 16/3/27.
//
//

#include "MyContactSolver.h"
#include "GJKDistance.h"
#include "MyShape.h"


MyContactSolver::MyContactSolver()
: m_positionIter(1)
{
    
}

MyContactSolver::~MyContactSolver()
{
    
}

void MyContactSolver::setPositionIter(int positionIter)
{
    m_positionIter = positionIter;
}

void MyContactSolver::solve(list<MyContact *> &l)
{
    list<MyContact*>::iterator it =  l.begin();
    for (; it != l.end(); it ++)
    {
        MyContact* contact = *it;
        if (contact->getBodyA()->bIsSensor() || contact->getBodyB()->bIsSensor()) continue;
        
        if (contact->getType() == MyContact::e_contact_edge)
        {
            solveEdgeContact(contact);
        }
        else
        {
            solveNormalContact(contact);
        }
        solvePositionConstrain(contact);
    }
    
}

void MyContactSolver::solveEdgeContact(MyContact* contact)
{
    MyBody* bodya = contact->getBodyA();
    MyBody* bodyb = contact->getBodyB();
    MyBody* edge  = NULL;
    MyBody* body = NULL;
    if (bodya->getShapeInfo()->getType() == MyShape::e_shape_edge)
    {
        edge = bodya;
        body = bodyb;
    }
    else
    {
        edge = bodyb;
        body = bodya;
    }
    
    MyAABB rect = edge->getAABBInfo();
    MyVec2 v = rect.getLowerBound() - rect.getUpperBound();
    MyVec2 bodyv = body->getLinearVelocity();
    if (v.x * v.x < 10)
    {
        body->setLinearVelocity(MyVec2(-bodyv.x, bodyv.y));
    }
    else
    {
        body->setLinearVelocity(MyVec2(bodyv.x, -bodyv.y));
    }
}

void MyContactSolver::solveNormalContact(MyContact* contact)
{
    MyBody* bodya = contact->getBodyA();
    MyBody* bodyb = contact->getBodyB();
    MyTransform ta = bodya->getTransformInfo();
    MyTransform tb = bodyb->getTransformInfo();
    // normal axis
    MyVec2 dx = MyVec2(1,0);
    MyVec2 dy = MyVec2(0,1);
    MyVec2 dnormal = ta.p - tb.p;
    dnormal.Normalize();
    // tangent axis
    MyVec2 dtangent = Cross(dnormal, 1);
    
    // for bodya
    MyVec2 xaxisv = bodya->getLinearVelocity().x * dx;
    MyVec2 yaxisv = bodya->getLinearVelocity().y * dy;
    MyVec2 normalaxisv = (xaxisv * dnormal + yaxisv * dnormal) * dnormal;
    MyVec2 tangentaxisv = (xaxisv * dtangent + yaxisv * dtangent) * dtangent;
    
    
    // for bodyb
    MyVec2 xaxisv2 = bodyb->getLinearVelocity().x * dx;
    MyVec2 yaxisv2 = bodyb->getLinearVelocity().y * dy;
    MyVec2 normalaxisv2 = (xaxisv2 * dnormal + yaxisv2 * dnormal) * dnormal;
    MyVec2 tangentaxisv2 = (xaxisv2 * dtangent + yaxisv2 * dtangent) * dtangent;
    
    // exchange the speed on normalaxis
    // for bodya, use normalaxisv2 and tangentaxisv
    MyVec2 newxaxisv = (normalaxisv2 * dx + tangentaxisv * dx) * dx;
    MyVec2 newyaxisv = (normalaxisv2 * dy + tangentaxisv * dy) * dy;
    
    // for bodyb, use normalaxisv and tangent
    MyVec2 newxaxisv2 = (normalaxisv * dx + tangentaxisv2 * dx) * dx;
    MyVec2 newyaxisv2 = (normalaxisv * dy + tangentaxisv2 * dy) * dy;
    
    
    bodya->setLinearVelocity(MyVec2(newxaxisv.x, newyaxisv.y));
    bodyb->setLinearVelocity(MyVec2(newxaxisv2.x, newyaxisv2.y));
    
    
}

bool MyContactSolver::TestOverlap(MyContact* c)
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

void MyContactSolver::solvePositionConstrain(MyContact* contact)
{
    for (int i = 0; i < m_positionIter; i ++)
    {
        if (TestOverlap(contact))
        {
            contact->getBodyA()->update();
            contact->getBodyB()->update();
        }
    }
}