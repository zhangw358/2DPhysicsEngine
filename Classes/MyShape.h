//
//  MyShape.h
//  PhysicsProj
//
//  Created by 张唯 on 16/3/21.
//
//

#ifndef MyShape_h
#define MyShape_h

#include "MyMathDef.h"
#include "MyAABB.h"
#include <vector>
using std::vector;

#define MypolygonRadius 0.1f

class MyShape
{
public:
    enum Type
    {
        e_shape_edge = 0,
        e_shape_polygon = 1,
        e_shape_circle = 2,
    };
    
    MyShape(Type type);
    virtual ~MyShape();
    virtual MyAABB getFatAABB(const MyTransform&) = 0;
    Type getType();
    float getRadius();
    
protected:
    Type m_type;
    float m_radius;
};


class MyCircleShape: public MyShape
{
public:
    MyCircleShape();
    virtual ~MyCircleShape();
    MyAABB getFatAABB(const MyTransform&);
    
    void setPosition(MyVec2 pos);
    MyVec2 getPosition();
    
    void setRadius(float r);
    
private:
    MyVec2 m_position;
};

class MyPolygonShape: public MyShape
{
public:
    MyPolygonShape();
    virtual ~MyPolygonShape();
    MyAABB getFatAABB(const MyTransform&);
    
    void setPosition(MyVec2 pos);
    MyVec2 getPosition();
    
    void setAsBox(float hx, float hy);
    
    void setVertex(vector<MyVec2> vecVertex);
    std::vector<MyVec2> getVertex();
    int getVertexCount();
    
private:
    MyVec2 m_position;
    vector<MyVec2> m_vecVertex;
};

class MyEdgeShape: public MyShape
{
public:
    MyEdgeShape();
    virtual ~MyEdgeShape();
    MyAABB getFatAABB(const MyTransform&);
    
    MyVec2 getP1();
    MyVec2 getP2();
    void setPosition(MyVec2 p1, MyVec2 p2);
private:
    MyVec2 p1;
    MyVec2 p2;
};
#endif /* MyShape_h */
