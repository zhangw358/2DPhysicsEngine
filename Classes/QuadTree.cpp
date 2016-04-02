//
//  QuadTree.cpp
//  PhysicsProj
//
//  Created by 张唯 on 16/3/21.
//
//

#include "QuadTree.h"
#include "MyShape.h"
#include "VisibleRect.h"

QuadTree::QuadTree()
: m_depth(3)
, m_sequence(0)
{
    MyVec2 lower;
    lower.setZero();
    MyVec2 upper = MyVec2(VisibleRect::rightTop().x, VisibleRect::rightTop().y);
    buildTree(MyAABB(lower, upper));
}

QuadTree::~QuadTree()
{
    
}

void QuadTree::buildTree(MyAABB worldRect)
{
    treeNode[0].Rect = worldRect;
    for (int i = 0; i < MAX_QUAD_TREE_NODE_NUM; i ++)
    {
        int child1 = i * QUAD_TREE_CHILD_NUM + 1;
        int child2 = i * QUAD_TREE_CHILD_NUM + 2;
        int child3 = i * QUAD_TREE_CHILD_NUM + 3;
        int child4 = i * QUAD_TREE_CHILD_NUM + 4;
        if (child1 >= MAX_QUAD_TREE_NODE_NUM) break;
        MyAABB rect = treeNode[i].Rect;
        MyVec2 rightTop = rect.getUpperBound();
        MyVec2 leftBottom = rect.getLowerBound();
        
        float middleX = (leftBottom.x + rightTop.x) / 2;
        float middleY = (leftBottom.y + rightTop.y) / 2;
        
        MyVec2 middle = MyVec2(middleX, middleY);
        MyVec2 middleTop = MyVec2(middleX, rightTop.y);
        MyVec2 middleBottom = MyVec2(middleX, leftBottom.y);
        MyVec2 middleRight = MyVec2(rightTop.x, middleY);
        MyVec2 middleLeft = MyVec2(leftBottom.x, middleY);
        
        treeNode[child1].Rect = MyAABB(middleLeft, middleTop);
        treeNode[child2].Rect = MyAABB(middle, rightTop);
        treeNode[child3].Rect = MyAABB(middleBottom, middleRight);
        treeNode[child4].Rect = MyAABB(leftBottom, middle);
    }
}

void QuadTree::insertUserData(MyBody *userData, MyAABB rect, int curIndex)
{
    for(;;)
    {
        for (int j = 1; j <= QUAD_TREE_CHILD_NUM; j++)
        {
            int childIndex = curIndex * QUAD_TREE_CHILD_NUM + j;
            if (testAContainB(treeNode[childIndex].Rect, rect))
            {
                curIndex = childIndex;
            }
        }
        if (curIndex == 0 || curIndex > MAX_NONE_CHILD_INDEX)
        {
            break;
        }
    }
    treeNode[curIndex].vecUserData.push_back(userData);
}

void QuadTree::addUserData(MyBody *userData, MyAABB rect)
{
    int curIndex = 0;
    while(true)
    {
        bool bStepInto = false;
        for (int j = 1; j <= 4; j++)
        {
            int childIndex = curIndex * 4 + j;
            bool bContain = testAContainB(treeNode[childIndex].Rect, rect);
            if (bContain)
            {
                curIndex = childIndex;
                bStepInto = true;
                break;
            }
        }
        if (!bStepInto || curIndex > MAX_NONE_CHILD_INDEX)
        {
            break;
        }
    }
    treeNode[curIndex].vecUserData.push_back(userData);
}

void QuadTree::checkContact()
{
    int i = MAX_QUAD_TREE_NODE_NUM - 1; // leaf index;
    for (; i >= 0; i --)
    {
        int index = i;
        for (; index >= 0; index = (index - 1) / QUAD_TREE_CHILD_NUM) // father index;
        {
            getContactListInNodes(i, index);
            if (index == 0)
            {
                break;
            }
        }
    }
}

list<MyContact*>& QuadTree::getAABBContactList()
{
    m_AABBContactList.sort();
    list<MyContact*>::iterator it = m_AABBContactList.begin();
    
    // remove duplicate contact;
    for (; it != m_AABBContactList.end(); it++)
    {
        while (true)
        {
            list<MyContact*>::iterator it2 = it;
            it2 ++;
            if (it2 == m_AABBContactList.end())
            {
                break;
            }
            
            if ((*it2)->getBodyA() != (*it)->getBodyA() || (*it2)->getBodyB() != (*it)->getBodyB())
            {
                break;
            }
            else
            {
                it = m_AABBContactList.erase(it);
            }
        }
    }
    
    return m_AABBContactList;
}

void QuadTree::getContactListInNodes(int Idx1, int Idx2)
{
    QuadTreeNode node1 = treeNode[Idx1];
    QuadTreeNode node2 = treeNode[Idx2];
    int length1 = (int)node1.vecUserData.size();
    int length2 = (int)node2.vecUserData.size();
    for (int i = 0; i < length1; i++)
    {
        MyBody* body1 = node1.vecUserData.at(i);
        for (int j = 0; j < length2; j++)
        {
            MyBody* body2 = node2.vecUserData.at(j);
            if (body1 != body2 && testOverlap(body1->getAABBInfo(), body2->getAABBInfo()))
            {
                // add Contact;
                MyShape::Type type1 = body1->getShapeInfo()->getType();
                MyShape::Type type2 = body2->getShapeInfo()->getType();
                if (type1 == MyShape::e_shape_edge || type2 == MyShape::e_shape_edge)
                {
                    m_AABBContactList.push_back(new EdgeContact(body1, body2, m_sequence));
                }
                else
                {
                    m_AABBContactList.push_back(new NormalContact(body1, body2, m_sequence));
                }
            }
        }
    }
}

void QuadTree::clearTree()
{
    m_sequence++;
    for (int i = 0; i < MAX_QUAD_TREE_NODE_NUM; i++)
    {
        treeNode[i].vecUserData.clear();
    }
    m_AABBContactList.clear();
}

bool QuadTree::testOverlap(MyAABB a, MyAABB b)
{
    MyVec2 d1, d2;
    d1 = b.getLowerBound() - a.getUpperBound();
    d2 = a.getLowerBound() - b.getUpperBound();
    
    if (d1.x > 0.0f || d1.y > 0.0f)
        return false;
    
    if (d2.x > 0.0f || d2.y > 0.0f)
        return false;
    
    return true;
}

bool QuadTree::testAContainB(const MyAABB& a, const MyAABB& b) const 
{
    MyVec2 d1, d2;
    d1 = b.getLowerBound() - a.getLowerBound();
    d2 = a.getUpperBound() - b.getUpperBound();
    if (d1.x > 0.0f && d1.y > 0.0f && d2.x > 0.0f && d2.y > 0.0f)
        return true;
    else
        return false;
}