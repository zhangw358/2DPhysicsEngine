//
//  GJKDistance.cpp
//  PhysicsProj
//
//  Created by 张唯 on 16/3/23.
//
//

#include "GJKDistance.h"
#include "MyShape.h"

// GJK using Voronoi regions (Christer Ericson) and Barycentric coordinates.
int gjkCalls, gjkIters, gjkMaxIters;

void GJKDistanceProxy::set(MyShape* shape)
{
    switch (shape->getType())
    {
        case MyShape::e_shape_circle:
        {
            MyCircleShape* circle = (MyCircleShape*)shape;
            m_vertices.push_back(circle->getPosition());
            m_count = 1;
            m_radius = circle->getRadius();
        }
            break;
            
        case MyShape::e_shape_polygon:
        {
            MyPolygonShape* polygon = (MyPolygonShape*)shape;
            m_vertices = polygon->getVertex();
            m_count = (int) polygon->getVertex().size();
            m_radius = polygon->getRadius();
        }
            break;
            
        case MyShape::e_shape_edge:
        {
            MyEdgeShape* edge = (MyEdgeShape*)shape;
            m_vertices.push_back(edge->getP1());
            m_vertices.push_back(edge->getP2());
            m_count = 2;
            m_radius = edge->getRadius();
        }
            break;
            
        default:
            assert(false);
    }
}


struct GJKSimplexVertex
{
    MyVec2 wA;        // support point in proxyA
    MyVec2 wB;        // support point in proxyB
    MyVec2 w;        // wB - wA
    float a;        // barycentric coordinate for closest point
    int indexA;    // wA index
    int indexB;    // wB index
};

struct GJKSimplex
{
    void readCache(    const GJKSimplexCache* cache,
                   const GJKDistanceProxy* proxyA, const MyTransform& transformA,
                   const GJKDistanceProxy* proxyB, const MyTransform& transformB)
    {
        assert(cache->count <= 3);
        
        // Copy data from cache.
        m_count = cache->count;
        GJKSimplexVertex* vertices = &m_v1;
        for (int i = 0; i < m_count; ++i)
        {
            GJKSimplexVertex* v = vertices + i;
            v->indexA = cache->indexA[i];
            v->indexB = cache->indexB[i];
            MyVec2 wALocal = proxyA->getVertex(v->indexA);
            MyVec2 wBLocal = proxyB->getVertex(v->indexB);
            v->wA = Mul(transformA, wALocal);
            v->wB = Mul(transformB, wBLocal);
            v->w = v->wB - v->wA;
            v->a = 0.0f;
        }
        
        // Compute the new simplex metric, if it is substantially different than
        // old metric then flush the simplex.
        if (m_count > 1)
        {
            float metric1 = cache->metric;
            float metric2 = getMetric();
            if (metric2 < 0.5f * metric1 || 2.0f * metric1 < metric2 || metric2 < FLT_EPSILON)
            {
                // Reset the simplex.
                m_count = 0;
            }
        }
        
        // If the cache is empty or invalid ...
        if (m_count == 0)
        {
            GJKSimplexVertex* v = vertices + 0;
            v->indexA = 0;
            v->indexB = 0;
            MyVec2 wALocal = proxyA->getVertex(0);
            MyVec2 wBLocal = proxyB->getVertex(0);
            v->wA = Mul(transformA, wALocal);
            v->wB = Mul(transformB, wBLocal);
            v->w = v->wB - v->wA;
            m_count = 1;
        }
    }
    
    void writeCache(GJKSimplexCache* cache) const
    {
        cache->metric = getMetric();
        cache->count = int(m_count);
        const GJKSimplexVertex* vertices = &m_v1;
        for (int i = 0; i < m_count; ++i)
        {
            cache->indexA[i] = int(vertices[i].indexA);
            cache->indexB[i] = int(vertices[i].indexB);
        }
    }
    
    MyVec2 getSearchDirection() const
    {
        switch (m_count)
        {
            case 1:
                return -m_v1.w;
                
            case 2:
            {
                MyVec2 e12 = m_v2.w - m_v1.w;
                float sgn = Cross(e12, -m_v1.w);
                if (sgn > 0.0f)
                {
                    // Origin is left of e12.
                    return Cross(1.0f, e12);
                }
                else
                {
                    // Origin is right of e12.
                    return Cross(e12, 1.0f);
                }
            }
                
            default:
                assert(false);
                return MyVec2(0.0f, 0.0f);
        }
    }
    
    MyVec2 getClosestPoint() const
    {
        switch (m_count)
        {
            case 0:
                assert(false);
                return MyVec2(0.0f,0.0f);
                
            case 1:
                return m_v1.w;
                
            case 2:
                return m_v1.a * m_v1.w + m_v2.a * m_v2.w;
                
            case 3:
                return MyVec2(0.0f,0.0f);
                
            default:
                assert(false);
                return MyVec2(0.0f,0.0f);
        }
    }
    
    void getWitnessPoints(MyVec2* pA, MyVec2* pB) const
    {
        switch (m_count)
        {
            case 0:
                assert(false);
                break;
                
            case 1:
                *pA = m_v1.wA;
                *pB = m_v1.wB;
                break;
                
            case 2:
                *pA = m_v1.a * m_v1.wA + m_v2.a * m_v2.wA;
                *pB = m_v1.a * m_v1.wB + m_v2.a * m_v2.wB;
                break;
                
            case 3:
                *pA = m_v1.a * m_v1.wA + m_v2.a * m_v2.wA + m_v3.a * m_v3.wA;
                *pB = *pA;
                break;
                
            default:
                assert(false);
                break;
        }
    }
    
    float getMetric() const
    {
        switch (m_count)
        {
            case 0:
                assert(false);
                return 0.0;
                
            case 1:
                return 0.0f;
                
            case 2:
                return MyDistance(m_v1.w, m_v2.w);
                
            case 3:
                return Cross(m_v2.w - m_v1.w, m_v3.w - m_v1.w);
                
            default:
                assert(false);
                return 0.0f;
        }
    }
    
    void Solve2();
    void Solve3();
    
    GJKSimplexVertex m_v1, m_v2, m_v3;
    int m_count;
};


// Solve a line segment using barycentric coordinates.
//
// p = a1 * w1 + a2 * w2
// a1 + a2 = 1
//
// The vector from the origin to the closest point on the line is
// perpendicular to the line.
// e12 = w2 - w1
// dot(p, e) = 0
// a1 * dot(w1, e) + a2 * dot(w2, e) = 0
//
// 2-by-2 linear system
// [1      1     ][a1] = [1]
// [w1.e12 w2.e12][a2] = [0]
//
// Define
// d12_1 =  dot(w2, e12)
// d12_2 = -dot(w1, e12)
// d12 = d12_1 + d12_2
//
// Solution
// a1 = d12_1 / d12
// a2 = d12_2 / d12
void GJKSimplex::Solve2()
{
    MyVec2 w1 = m_v1.w;
    MyVec2 w2 = m_v2.w;
    MyVec2 e12 = w2 - w1;
    
    // w1 region
    float d12_2 = -Dot(w1, e12);
    if (d12_2 <= 0.0f)
    {
        // a2 <= 0, so we clamp it to 0
        m_v1.a = 1.0f;
        m_count = 1;
        return;
    }
    
    // w2 region
    float d12_1 = Dot(w2, e12);
    if (d12_1 <= 0.0f)
    {
        // a1 <= 0, so we clamp it to 0
        m_v2.a = 1.0f;
        m_count = 1;
        m_v1 = m_v2;
        return;
    }
    
    // Must be in e12 region.
    float inv_d12 = 1.0f / (d12_1 + d12_2);
    m_v1.a = d12_1 * inv_d12;
    m_v2.a = d12_2 * inv_d12;
    m_count = 2;
}

// Possible regions:
// - points[2]
// - edge points[0]-points[2]
// - edge points[1]-points[2]
// - inside the triangle
void GJKSimplex::Solve3()
{
    MyVec2 w1 = m_v1.w;
    MyVec2 w2 = m_v2.w;
    MyVec2 w3 = m_v3.w;
    
    // Edge12
    // [1      1     ][a1] = [1]
    // [w1.e12 w2.e12][a2] = [0]
    // a3 = 0
    MyVec2 e12 = w2 - w1;
    float w1e12 = Dot(w1, e12);
    float w2e12 = Dot(w2, e12);
    float d12_1 = w2e12;
    float d12_2 = -w1e12;
    
    // Edge13
    // [1      1     ][a1] = [1]
    // [w1.e13 w3.e13][a3] = [0]
    // a2 = 0
    MyVec2 e13 = w3 - w1;
    float w1e13 = Dot(w1, e13);
    float w3e13 = Dot(w3, e13);
    float d13_1 = w3e13;
    float d13_2 = -w1e13;
    
    // Edge23
    // [1      1     ][a2] = [1]
    // [w2.e23 w3.e23][a3] = [0]
    // a1 = 0
    MyVec2 e23 = w3 - w2;
    float w2e23 = Dot(w2, e23);
    float w3e23 = Dot(w3, e23);
    float d23_1 = w3e23;
    float d23_2 = -w2e23;
    
    // Triangle123
    float n123 = Cross(e12, e13);
    
    float d123_1 = n123 * Cross(w2, w3);
    float d123_2 = n123 * Cross(w3, w1);
    float d123_3 = n123 * Cross(w1, w2);
    
    // w1 region
    if (d12_2 <= 0.0f && d13_2 <= 0.0f)
    {
        m_v1.a = 1.0f;
        m_count = 1;
        return;
    }
    
    // e12
    if (d12_1 > 0.0f && d12_2 > 0.0f && d123_3 <= 0.0f)
    {
        float inv_d12 = 1.0f / (d12_1 + d12_2);
        m_v1.a = d12_1 * inv_d12;
        m_v2.a = d12_2 * inv_d12;
        m_count = 2;
        return;
    }
    
    // e13
    if (d13_1 > 0.0f && d13_2 > 0.0f && d123_2 <= 0.0f)
    {
        float inv_d13 = 1.0f / (d13_1 + d13_2);
        m_v1.a = d13_1 * inv_d13;
        m_v3.a = d13_2 * inv_d13;
        m_count = 2;
        m_v2 = m_v3;
        return;
    }
    
    // w2 region
    if (d12_1 <= 0.0f && d23_2 <= 0.0f)
    {
        m_v2.a = 1.0f;
        m_count = 1;
        m_v1 = m_v2;
        return;
    }
    
    // w3 region
    if (d13_1 <= 0.0f && d23_1 <= 0.0f)
    {
        m_v3.a = 1.0f;
        m_count = 1;
        m_v1 = m_v3;
        return;
    }
    
    // e23
    if (d23_1 > 0.0f && d23_2 > 0.0f && d123_1 <= 0.0f)
    {
        float inv_d23 = 1.0f / (d23_1 + d23_2);
        m_v2.a = d23_1 * inv_d23;
        m_v3.a = d23_2 * inv_d23;
        m_count = 2;
        m_v1 = m_v3;
        return;
    }
    
    // Must be in triangle123
    float inv_d123 = 1.0f / (d123_1 + d123_2 + d123_3);
    m_v1.a = d123_1 * inv_d123;
    m_v2.a = d123_2 * inv_d123;
    m_v3.a = d123_3 * inv_d123;
    m_count = 3;
}

void GJKDistance(GJKDistanceOutput* output,
                GJKSimplexCache* cache,
                const GJKDistanceInput* input)
{
    ++gjkCalls;
    
    const GJKDistanceProxy* proxyA = &input->proxyA;
    const GJKDistanceProxy* proxyB = &input->proxyB;
    
    MyTransform transformA = input->transformA;
    MyTransform transformB = input->transformB;
    
    // Initialize the simplex.
    GJKSimplex simplex;
    simplex.readCache(cache, proxyA, transformA, proxyB, transformB);
    
    // Get simplex vertices as an array.
    GJKSimplexVertex* vertices = &simplex.m_v1;
    const int k_maxIters = 20;
    
    // These store the vertices of the last simplex so that we
    // can check for duplicates and prevent cycling.
    int saveA[3], saveB[3];
    int saveCount = 0;
    
    MyVec2 closestPoint = simplex.getClosestPoint();
    float distanceSqr1 = closestPoint.LengthSquared();
    float distanceSqr2 = distanceSqr1;
    
    // Main iteration loop.
    int iter = 0;
    while (iter < k_maxIters)
    {
        // Copy simplex so we can identify duplicates.
        saveCount = simplex.m_count;
        for (int i = 0; i < saveCount; ++i)
        {
            saveA[i] = vertices[i].indexA;
            saveB[i] = vertices[i].indexB;
        }
        
        switch (simplex.m_count)
        {
            case 1:
                break;
                
            case 2:
                simplex.Solve2();
                break;
                
            case 3:
                simplex.Solve3();
                break;
                
            default:
                assert(false);
        }
        
        // If we have 3 points, then the origin is in the corresponding triangle.
        if (simplex.m_count == 3)
        {
            break;
        }
        
        // Compute closest point.
        MyVec2 p = simplex.getClosestPoint();
        distanceSqr2 = p.LengthSquared();
        
        // Ensure progress
        if (distanceSqr2 >= distanceSqr1)
        {
            //break;
        }
        distanceSqr1 = distanceSqr2;
        
        // Get search direction.
        MyVec2 d = simplex.getSearchDirection();
        
        // Ensure the search direction is numerically fit.
        if (d.LengthSquared() < FLT_EPSILON * FLT_EPSILON)
        {
            // The origin is probably contained by a line segment
            // or triangle. Thus the shapes are overlapped.
            
            // We can't return zero here even though there may be overlap.
            // In case the simplex is a point, segment, or triangle it is difficult
            // to determine if the origin is contained in the CSO or very close to it.
            break;
        }
        
        // Compute a tentative new simplex vertex using support points.
        GJKSimplexVertex* vertex = vertices + simplex.m_count;
        vertex->indexA = proxyA->getSupport(MulT(transformA.q, -d));
        vertex->wA = Mul(transformA, proxyA->getVertex(vertex->indexA));
        MyVec2 wBLocal;
        vertex->indexB = proxyB->getSupport(MulT(transformB.q, d));
        vertex->wB = Mul(transformB, proxyB->getVertex(vertex->indexB));
        vertex->w = vertex->wB - vertex->wA;
        
        // Iteration count is equated to the number of support point calls.
        ++iter;
        ++gjkIters;
        
        // Check for duplicate support points. This is the main termination criteria.
        bool duplicate = false;
        for (int i = 0; i < saveCount; ++i)
        {
            if (vertex->indexA == saveA[i] && vertex->indexB == saveB[i])
            {
                duplicate = true;
                break;
            }
        }
        
        // If we found a duplicate support point we must exit to avoid cycling.
        if (duplicate)
        {
            break;
        }
        
        // New vertex is ok and needed.
        ++simplex.m_count;
    }
    
    gjkMaxIters = Max(gjkMaxIters, iter);
    
    // Prepare output.
    simplex.getWitnessPoints(&output->pointA, &output->pointB);
    output->distance = MyDistance(output->pointA, output->pointB);
    output->iterations = iter;
    
    // Cache the simplex.
    simplex.writeCache(cache);
    
    // Apply radii if requested.
    if (input->useRadii)
    {
        float rA = proxyA->m_radius;
        float rB = proxyB->m_radius;
        
        if (output->distance > rA + rB && output->distance > FLT_EPSILON)
        {
            // Shapes are still no overlapped.
            // Move the witness points to the outer surface.
            output->distance -= rA + rB;
            MyVec2 normal = output->pointB - output->pointA;
            normal.Normalize();
            output->pointA += rA * normal;
            output->pointB -= rB * normal;
        }
        else
        {
            // Shapes are overlapped when radii are considered.
            // Move the witness points to the middle.
            MyVec2 p = 0.5f * (output->pointA + output->pointB);
            output->pointA = p;
            output->pointB = p;
            output->distance = 0.0f;
        }
    }
}
