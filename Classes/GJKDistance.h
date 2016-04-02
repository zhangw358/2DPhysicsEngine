//
//  GJKDistance.h
//  PhysicsProj
//
//  Created by 张唯 on 16/3/23.
//
//

#ifndef GJKDistance_h
#define GJKDistance_h
#include "MyMathDef.h"
#include "Assert.h"
#include <cstdio>
#include <vector>
class MyShape;

using std::vector;

/// A distance proxy is used by the GJK algorithm.
/// It encapsulates any shape.
struct GJKDistanceProxy
{
    GJKDistanceProxy() : m_count(0), m_radius(0.0f) {}
    
    /// Initialize the proxy using the given shape. The shape
    /// must remain in scope while the proxy is in use.
    void set(MyShape* shape);
    
    /// Get the supporting vertex index in the given direction.
    int getSupport(const MyVec2& d) const;
    
    /// Get the supporting vertex in the given direction.
    const MyVec2& getSupportVertex(const MyVec2& d) const;
    
    /// Get the vertex count.
    int getVertexCount() const;
    
    /// Get a vertex by index. Used by b2Distance.
    const MyVec2& getVertex(int index) const;
    
    vector<MyVec2> m_vertices;
    int m_count;
    float m_radius;
};

/// Used to warm start b2Distance.
/// Set count to zero on first call.
struct GJKSimplexCache
{
    float metric;        ///< length or area
    int count;
    int indexA[3];    ///< vertices on shape A
    int indexB[3];    ///< vertices on shape B
};

/// Input for b2Distance.
/// You have to option to use the shape radii
/// in the computation. Even
struct GJKDistanceInput
{
    GJKDistanceProxy proxyA;
    GJKDistanceProxy proxyB;
    MyTransform transformA;
    MyTransform transformB;
    bool useRadii;
};

/// Output for b2Distance.
struct GJKDistanceOutput
{
    MyVec2 pointA;        ///< closest point on shapeA
    MyVec2 pointB;        ///< closest point on shapeB
    float distance;
    int iterations;    ///< number of GJK iterations used
};

/// Compute the closest points between two shapes. Supports any combination of:
/// b2CircleShape, b2PolygonShape, b2EdgeShape. The simplex cache is input/output.
/// On the first call set b2SimplexCache.count to zero.
void GJKDistance(GJKDistanceOutput* output,
                GJKSimplexCache* cache,
                const GJKDistanceInput* input);


//////////////////////////////////////////////////////////////////////////

inline int GJKDistanceProxy::getVertexCount() const
{
    return m_count;
}

inline const MyVec2& GJKDistanceProxy::getVertex(int index) const
{
    assert(0 <= index && index < m_count);
    return m_vertices[index];
}

inline int GJKDistanceProxy::getSupport(const MyVec2& d) const
{
    int bestIndex = 0;
    float bestValue = Dot(m_vertices[0], d);
    for (int i = 1; i < m_count; ++i)
    {
        float value = Dot(m_vertices[i], d);
        if (value > bestValue)
        {
            bestIndex = i;
            bestValue = value;
        }
    }
    
    return bestIndex;
}

inline const MyVec2& GJKDistanceProxy::getSupportVertex(const MyVec2& d) const
{
    int bestIndex = 0;
    float bestValue = Dot(m_vertices[0], d);
    for (int i = 1; i < m_count; ++i)
    {
        float value = Dot(m_vertices[i], d);
        if (value > bestValue)
        {
            bestIndex = i;
            bestValue = value;
        }
    }
    
    return m_vertices[bestIndex];
}

#endif /* GJKDistance_h */
