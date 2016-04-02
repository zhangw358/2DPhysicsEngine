//
//  MyBody.cpp
//  PhysicsProj
//
//  Created by 张唯 on 16/3/16.
//
//

#include "MyBody.h"
#include "MyShape.h"
#include "MyMathDef.h"

MyBody::MyBody()
{
    m_linearVelocity.setZero();
    m_position.setZero();
    m_angularVelocity = 0;
    m_angle = 0;
    m_transform.setIdentity();
    m_linearDamping = 0.995;
    m_bValid = true;
    m_userData = NULL;
    m_bSensor = false;
}

MyBody::~MyBody()
{
   if (m_shape)
   {
       delete m_shape;
   }
}

void MyBody::update()
{
    m_position += m_linearVelocity;
    m_angle += m_angularVelocity;
    m_transform.p = m_position;
    m_transform.q.set(m_angle);
    m_linearVelocity *= m_linearDamping;
}

MyShape* MyBody::getShapeInfo()
{
    return m_shape;
}

MyTransform MyBody::getTransformInfo()
{
    return m_transform;
}

void MyBody::setLinearVelocity(MyVec2 v)
{
    m_linearVelocity = v;
}

MyVec2 MyBody::getLinearVelocity() 
{
    return m_linearVelocity;
}

void MyBody::setAngularVelocity(float w)
{
    m_angularVelocity = w;
}

float MyBody::getAngularVelocity()
{
    return m_angularVelocity;
}

void MyBody::setAngle(float a)
{
    m_angle = a;
    m_transform.q.set(a);
}

float MyBody::getAngle()
{
    return m_angle;
}

void MyBody::setPosition(MyVec2 v)
{
    m_position = v;
    m_transform.p = v;
}

MyVec2 MyBody::getPosition()
{
    return m_position;
}

void MyBody::createShape(MyShape *shape)
{
    switch (shape->getType())
    {
        case MyShape::e_shape_circle:
        {
            MyCircleShape *source = (MyCircleShape*)shape;
            MyCircleShape *dst = new MyCircleShape();
            dst->setPosition(source->getPosition());
            dst->setRadius(source->getRadius());
            m_shape = dst;
        }
        break;
        case MyShape::e_shape_edge:
        {
            MyEdgeShape *source = (MyEdgeShape*) shape;
            MyEdgeShape *dst = new MyEdgeShape();
            dst->setPosition(source->getP1(), source->getP2());
            m_shape = dst;
        }
        break;
        case MyShape::e_shape_polygon:
        {
            MyPolygonShape *source = (MyPolygonShape*) shape;
            MyPolygonShape *dst = new MyPolygonShape();
            dst->setPosition(source->getPosition());
            dst->setVertex(source->getVertex());
            m_shape = dst;
        }
        break;
    }
}

MyAABB MyBody::getAABBInfo()
{
    return m_shape->getFatAABB(m_transform);
}

void MyBody::setUserData(void * data)
{
    m_userData = data;
}

void* MyBody::getUserData()
{
    return m_userData;
}

void MyBody::setValid(bool bValid)
{
    m_bValid = bValid;
}

bool MyBody::getValid()
{
    return m_bValid;
}

void MyBody::setSensor(bool bSensor)
{
    m_bSensor = bSensor;
}

bool MyBody::bIsSensor()
{
    return m_bSensor;
}

void MyBody::setLinearDamping(float value)
{
    m_linearDamping = value;
}

float MyBody::getLinearDamping()
{
    return m_linearDamping;
}