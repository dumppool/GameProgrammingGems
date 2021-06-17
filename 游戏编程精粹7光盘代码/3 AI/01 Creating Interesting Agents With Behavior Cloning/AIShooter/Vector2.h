#ifndef __AISHOOTER_VECTOR2_H__
#define __AISHOOTER_VECTOR2_H__

// Tell the MSVC version to define math constants
#define _USE_MATH_DEFINES
#include <math.h>

/**
 * Simple 2D vector class. Contains only the functions needed for this demo.
 */
class Vector2
{
public:
    float x, y;

    Vector2 () 
        : x (0), y (0)
    {
    }

    Vector2 (float ix, float iy)
        : x (ix), y (iy)
    {
    }

    void rotate (float angle)
    {
        float rad = (angle * (float) M_PI) / 180.0f;
        float c = cos (rad), s = sin (rad);
        float x2 = x * c - y * s;
        float y2 = y * c + x * s;
        x = x2;
        y = y2;
    }

    void normalize ()
    {
        float m = magnitudeSquared ();
        if(m > 0) {
            *this *= 1.0f / sqrtf (m);
        }
    }

    float dot (const Vector2& rhs)
    {
        return x * rhs.x + y * rhs.y;
    }

    float perpdot (const Vector2& rhs)
    {
        return x * rhs.y - y * rhs.x;
    }

    float angleBetween (const Vector2& rhs)
    {
        float angle = atan2(perpdot(rhs), dot(rhs));
        return (angle * 180.0f) / (float) M_PI;
    }

    float magnitude () const
    {
        return sqrtf (x * x + y * y);
    }

    float magnitudeSquared () const
    {
        return x * x + y * y;
    }

    Vector2 operator+(const Vector2& rhs) const
    {
        return Vector2 (x + rhs.x, y + rhs.y);
    }

    Vector2 operator-(const Vector2& rhs) const
    {
        return Vector2 (x - rhs.x, y - rhs.y);
    }

    Vector2 operator-() const
    {
        return Vector2 (-x, -y);
    }

    Vector2& operator+=(const Vector2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector2& operator*=(float rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }
};

#endif

