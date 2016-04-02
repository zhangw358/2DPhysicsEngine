//
//  MyContact.h
//  PhysicsProj
//
//  Created by 张唯 on 16/3/21.
//
//

#ifndef MyContact_h
#define MyContact_h
#include "MyBody.h"

class MyContact
{
public:
    
    enum Type
    {
        e_contact_edge = 0,
        e_contact_normal = 1,
    };
    
    MyContact(MyBody* a, MyBody* b, Type t, int CurSeq);
    
    MyContact(const MyContact& c);
    
    virtual ~MyContact();
    MyBody* getBodyA() const;
    MyBody* getBodyB() const;
    
    int getContactSeqence() const;
    bool operator < (const MyContact*& c) const;
    
    Type getType();
private:
    int seq;
    Type type;
    MyBody* bodya;
    MyBody* bodyb;
};

class EdgeContact : public MyContact
{
public:
    EdgeContact(MyBody* a, MyBody* b, int CurSeq);
    virtual ~EdgeContact();
};

class NormalContact : public MyContact
{
public:
    NormalContact(MyBody* a, MyBody* b, int CurSeq);
    virtual ~NormalContact();
};

#endif /* MyContact_h */
