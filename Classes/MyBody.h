//
//  MyBody.hpp
//  PhysicsProj
//
//  Created by 张唯 on 16/3/16.
//
//

#ifndef MyBody_h
#define MyBody_h

#include "MyMathDef.h"
class MyShape;
class MyAABB;

class MyBody
{
public:
    MyBody();
    ~MyBody();
    void createShape(MyShape* shape);
    MyShape* getShapeInfo();
    
    void setLinearVelocity(MyVec2 v);
    MyVec2 getLinearVelocity();
    
    void setAngularVelocity(float w);
    float getAngularVelocity();
    
    void setAngle(float a);
    float getAngle();
    
    void setPosition(MyVec2 v);
    MyVec2 getPosition();
    
    MyTransform getTransformInfo();
    void setTransformInfo(MyTransform t);
    
    void update();
    
    MyAABB getAABBInfo();
    
    void setUserData(void*);
    void* getUserData();
    
    void setValid(bool bValid);
    bool getValid();
    
    void setSensor(bool bSensor);
    bool bIsSensor();
    
    void setLinearDamping(float value);
    float getLinearDamping();
    
    void setGroupData(int data);
    int getGroupData();
private:
    int m_groupData;
    void* m_userData;
    bool m_bValid;
    bool m_bSensor;
    MyShape *m_shape;
    float m_angularVelocity;
    MyVec2 m_linearVelocity;
    float m_linearDamping;
    MyVec2 m_position;
    float m_angle;
    MyTransform m_transform;
};
#endif /* MyBody_hpp */
