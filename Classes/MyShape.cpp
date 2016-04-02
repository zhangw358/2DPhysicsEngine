//
//  MyShape.cpp
//  PhysicsProj
//
//  Created by 张唯 on 16/3/21.
//
//

#include "MyShape.h"


// Base Class: Shape
MyShape::Type MyShape::getType()
{
    return m_type;
}


float MyShape::getRadius()
{
    return m_radius;
}

MyShape::MyShape(MyShape::Type type)
: m_type(type)
{
}

MyShape::~MyShape()
{
}

// Super Class: Cricle
MyCircleShape::MyCircleShape()
: MyShape(MyShape::e_shape_circle)
{
    m_position.setZero();
    m_radius = 0;
}

MyCircleShape::~MyCircleShape()
{
}

MyAABB MyCircleShape::getFatAABB(const MyTransform& t)
{
    float x = t.p.x;
    float y = t.p.y;
    float r = m_radius * 1.1; // Fatter.
    MyVec2 l = MyVec2(x - r, y - r);
    MyVec2 u = MyVec2(x + r, y + r);
    MyAABB aabb(l, u);
    return aabb;
}

void MyCircleShape::setPosition(MyVec2 pos)
{
    m_position = pos;
}

MyVec2 MyCircleShape::getPosition()
{
    return m_position;
}

void MyCircleShape::setRadius(float r)
{
    m_radius = r;
}


// Super Class: Polygon
MyPolygonShape::MyPolygonShape()
: MyShape(MyShape::e_shape_polygon)
{
    m_vecVertex.clear();
    m_position.setZero();
    m_radius = MypolygonRadius;
}

MyPolygonShape::~MyPolygonShape()
{
    
}

void MyPolygonShape::setAsBox(float hx, float hy)
{
    m_vecVertex.push_back(MyVec2(-hx, -hy));
    m_vecVertex.push_back(MyVec2(hx, -hy));
    m_vecVertex.push_back(MyVec2(hx, hy));
    m_vecVertex.push_back(MyVec2(-hx, hy));
}

MyAABB MyPolygonShape::getFatAABB(const MyTransform& t)
{
    int nVertexCount = (int)m_vecVertex.size();
    
    MyVec2 lower = Mul(t, m_vecVertex[0]);
    MyVec2 upper = lower;
    
    for (int i = 1; i < nVertexCount; ++i)
    {
        MyVec2 v = Mul(t, m_vecVertex[i]);
        lower = Min(lower, v);
        upper = Max(upper, v);
    }
    
    MyAABB aabb(lower, upper);
    return aabb;
}

void MyPolygonShape::setPosition(MyVec2 pos)
{
    m_position = pos;
}

MyVec2 MyPolygonShape::getPosition()
{
    return m_position;
}

void MyPolygonShape::setVertex(vector<MyVec2> vecVertex)
{
    m_vecVertex = vecVertex;
}

std::vector<MyVec2> MyPolygonShape::getVertex()
{
    return m_vecVertex;
}

int MyPolygonShape::getVertexCount()
{
    int nCount = (int)m_vecVertex.size();
    return nCount;
}

// Super Class: Edge
MyEdgeShape::MyEdgeShape()
: MyShape(MyShape::e_shape_edge)
{
    m_radius = MypolygonRadius;
    p1.setZero();
    p2.setZero();
}

MyEdgeShape::~MyEdgeShape()
{
}

MyAABB MyEdgeShape::getFatAABB(const MyTransform& t)
{
    MyVec2 v1 = Mul(t, p1);
    MyVec2 v2 = Mul(t, p2);
    
    MyVec2 lower = Min(v1, v2);
    MyVec2 upper = Max(v1, v2);
    
    MyVec2 r(m_radius, m_radius);
    return MyAABB(lower-r, upper+r);
}

void MyEdgeShape::setPosition(MyVec2 v1, MyVec2 v2)
{
    p1 = v1;
    p2 = v2;
}

MyVec2 MyEdgeShape::getP1()
{
    return p1;
}

MyVec2 MyEdgeShape::getP2()
{
    return p2;
}