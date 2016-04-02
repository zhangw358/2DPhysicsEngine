//
//  MyMathDef.h
//  PhysicsProj
//
//  Created by 张唯 on 16/3/16.
//
//

#ifndef MyMathDef_h
#define MyMathDef_h


#include <cmath>
#include "float.h"

/// A 2D column vector.
struct MyVec2
{
    /// Default constructor does nothing (for performance).
    MyVec2() {}
    
    /// Construct using coordinates.
    MyVec2(float x, float y) : x(x), y(y) {}
    
    /// Set this vector to all zeros.
    void setZero() { x = 0.0f; y = 0.0f; }
    
    /// Set this vector to some specified coordinates.
    void set(float x_, float y_) { x = x_; y = y_; }
    
    /// Negate this vector.
    MyVec2 operator -() const { MyVec2 v; v.set(-x, -y); return v; }
    
    /// Read from and indexed element.
    float operator () (int i) const
    {
        return (&x)[i];
    }
    
    /// Write to an indexed element.
    float& operator () (int i)
    {
        return (&x)[i];
    }
    
    /// Add a vector to this vector.
    void operator += (const MyVec2& v)
    {
        x += v.x; y += v.y;
    }
    
    /// Subtract a vector from this vector.
    void operator -= (const MyVec2& v)
    {
        x -= v.x; y -= v.y;
    }
    
    /// Multiply this vector by a scalar.
    void operator *= (float a)
    {
        x *= a; y *= a;
    }
    
    /// Get the length of this vector (the norm).
    float Length() const
    {
        return std::sqrt(x * x + y * y);
    }
    
    /// Convert this vector into a unit vector. Returns the length.
    float Normalize()
    {
        float length = Length();
        if (length < FLT_EPSILON)
        {
            return 0.0f;
        }
        float invLength = 1.0f / length;
        x *= invLength;
        y *= invLength;
        
        return length;
    }
    
    /// Get the length squared. For performance, use this instead of
    /// b2Vec2::Length (if possible).
    float LengthSquared() const
    {
        return x * x + y * y;
    }
    
    float x, y;
};


/// Rotation
struct MyRot
{
    MyRot() {}
    
    /// Initialize from an angle in radians
    explicit MyRot(float angle)
    {
        /// TODO_ERIN optimize
        s = sinf(angle);
        c = cosf(angle);
    }
    
    /// Set using an angle in radians.
    void set(float angle)
    {
        /// TODO_ERIN optimize
        s = sinf(angle);
        c = cosf(angle);
    }
    
    /// Set to the identity rotation
    void setIdentity()
    {
        s = 0.0f;
        c = 1.0f;
    }
    
    /// Get the angle in radians
    float getAngle() const
    {
        return atan2(s, c);
    }
    
    /// Get the x-axis
    MyVec2 getXAxis() const
    {
        return MyVec2(c, s);
    }
    
    /// Get the u-axis
    MyVec2 getYAxis() const
    {
        return MyVec2(-s, c);
    }
    
    /// Sine and cosine
    float s, c;
};

// A transform contains translation and rotation. It is used to represent
/// the position and orientation of rigid frames.
struct MyTransform
{
    /// The default constructor does nothing.
    MyTransform() {}
    
    /// Initialize using a position vector and a rotation.
    MyTransform(const MyVec2& position, const MyRot& rotation) : p(position), q(rotation) {}
    
    /// Set this to the identity transform.
    void setIdentity()
    {
        p.setZero();
        q.setIdentity();
    }
    
    /// Set this based on the position and angle.
    void set(const MyVec2& position, float angle)
    {
        p = position;
        q.set(angle);
    }
    
    MyVec2 p;
    MyRot q;
};

template <typename T>
inline T Min(T a, T b)
{
    return a < b ? a : b;
}

inline MyVec2 Min(const MyVec2& a, const MyVec2& b)
{
    return MyVec2(Min(a.x, b.x), Min(a.y, b.y));
}

template <typename T>
inline T Max(T a, T b)
{
    return a > b ? a : b;
}


/// Add two vectors component-wise.
inline MyVec2 operator + (const MyVec2& a, const MyVec2& b)
{
    return MyVec2(a.x + b.x, a.y + b.y);
}

/// Subtract two vectors component-wise.
inline MyVec2 operator - (const MyVec2& a, const MyVec2& b)
{
    return MyVec2(a.x - b.x, a.y - b.y);
}

inline float operator * (const MyVec2& a, const MyVec2& b)
{
    return a.x * b.x + a.y * b.y;
}
inline MyVec2 operator * (float s, const MyVec2& a)
{
    return MyVec2(a.x * s, a.y * s);
}

inline MyVec2 Max(const MyVec2& a, const MyVec2& b)
{
    return MyVec2(Max(a.x, b.x), Max(a.y, b.y));
}

inline MyVec2 Mul(const MyTransform& T, const MyVec2& v)
{
    float x = (T.q.c * v.x - T.q.s * v.y) + T.p.x;
    float y = (T.q.s * v.x + T.q.c * v.y) + T.p.y;
    
    return MyVec2(x, y);
}

/// Perform the dot product on two vectors.
inline float Dot(const MyVec2& a, const MyVec2& b)
{
    return a.x * b.x + a.y * b.y;
}

/// Perform the cross product on two vectors. In 2D this produces a scalar.
inline float Cross(const MyVec2& a, const MyVec2& b)
{
    return a.x * b.y - a.y * b.x;
}

/// Perform the cross product on a vector and a scalar. In 2D this produces
/// a vector.
inline MyVec2 Cross(const MyVec2& a, float s)
{
    return MyVec2(s * a.y, -s * a.x);
}

/// Perform the cross product on a scalar and a vector. In 2D this produces
/// a vector.
inline MyVec2 Cross(float s, const MyVec2& a)
{
    return MyVec2(-s * a.y, s * a.x);
}

inline MyVec2 MulT(const MyTransform& T, const MyVec2& v)
{
    float px = v.x - T.p.x;
    float py = v.y - T.p.y;
    float x = (T.q.c * px + T.q.s * py);
    float y = (-T.q.s * px + T.q.c * py);
    
    return MyVec2(x, y);
}

/// Inverse rotate a vector
inline MyVec2 MulT(const MyRot& q, const MyVec2& v)
{
    return MyVec2(q.c * v.x + q.s * v.y, -q.s * v.x + q.c * v.y);
}

inline float MyDistance(const MyVec2& a, const MyVec2& b)
{
    MyVec2 c = a - b;
    return c.Length();
}

#endif /* MyMathDef_h */
