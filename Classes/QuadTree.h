//
//  QuadTree.h
//  PhysicsProj
//
//  Created by 张唯 on 16/3/21.
//
//

#ifndef QuadTree_h
#define QuadTree_h

#include "MyAABB.h"
#include "MyBody.h"
#include "MyContact.h"
#include <vector>
#include <list>
#define QUAD_TREE_CHILD_NUM 4
#define MAX_QUAD_TREE_LEVEL 4
#define MAX_NONE_CHILD_INDEX 4
#define MAX_QUAD_TREE_NODE_NUM 21

using std::vector;
using std::list;

class QuadTreeNode
{
public:
    MyAABB Rect;
    vector<MyBody*> vecUserData;
};


class QuadTree
{
public:
    QuadTree();
    ~QuadTree();
    void buildTree(MyAABB worldRect);
    void clearTree();
    void addUserData(MyBody* userData, MyAABB rect);
    void checkContact();
    list<MyContact*>& getAABBContactList();
    
private:
    bool testOverlap(MyAABB a, MyAABB b);
    bool testAContainB(const MyAABB& a, const MyAABB& b) const;
    void getContactListInNodes(int Idx1, int Idx2);
    void insertUserData(MyBody* userData, MyAABB rect, int curIndex);
private:
    int m_depth;
    int m_sequence;
    list<MyContact*> m_AABBContactList;
    QuadTreeNode treeNode[MAX_QUAD_TREE_NODE_NUM] ;
};

#endif /* QuadTree_h */
