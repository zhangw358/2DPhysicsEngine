//
//  MyContact.cpp
//  PhysicsProj
//
//  Created by 张唯 on 16/3/27.
//
//

#include "MyContact.h"

MyContact::MyContact(MyBody* a, MyBody* b, Type t, int CurSeq)
{
    if (a < b)
    {
        bodya = a;
        bodyb = b;
    }
    else
    {
        bodya = b;
        bodyb = a;
    }
    seq = CurSeq;
    type = t;
}

MyContact::MyContact(const MyContact& c)
{
    bodya = c.getBodyA();
    bodyb = c.getBodyB();
    seq = c.getContactSeqence();
}

MyContact::~MyContact()
{
    
}

MyBody* MyContact::getBodyA() const
{
    return bodya;
}

MyBody* MyContact::getBodyB() const
{
    return bodyb;
}

int MyContact::getContactSeqence() const
{
    return seq;
}

bool MyContact::operator < (const MyContact*& c) const
{
    if (bodya != c->getBodyA() )
    {
        return bodya < c->getBodyA();
    }
    if (bodyb != c->getBodyB())
    {
        return bodyb < c->getBodyB();
    }
    return seq > c->getContactSeqence();
}

MyContact::Type MyContact::getType()
{
    return type;
}

EdgeContact::EdgeContact(MyBody* a, MyBody* b, int CurSeq)
: MyContact(a,b,e_contact_edge, CurSeq)
{
}

EdgeContact::~EdgeContact()
{
}


NormalContact::NormalContact(MyBody* a, MyBody* b, int CurSeq)
: MyContact(a,b,e_contact_normal, CurSeq)
{
    
}

NormalContact::~NormalContact()
{
    
}