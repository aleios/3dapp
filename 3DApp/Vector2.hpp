#pragma once
#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <cmath>
#include <iostream>
#include <limits.h>

#include <SFML/System/Vector2.hpp>

#include "Vector3.hpp"

class Vector2
{
public:
    // Default Constructor
    explicit Vector2(float inX = 0.0f, float inY = 0.0f)
        : x(inX), y(inY)
    {
    }

    // Copy Constructor
    Vector2(const Vector2& other)
    {
        x = other.x;
        y = other.y;
    }

    // Assignment
    Vector2& operator=(const Vector2& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    // Arithmatic Operators
    Vector2& operator++()
    {
        x += 1.0f;
        y += 1.0f;
        return *this;
    }

    Vector2 operator++(int)
    {
        Vector2 tmp(*this);
        operator++();
        return tmp;
    }

    Vector2& operator--()
    {
        x -= 1.0f;
        y -= 1.0f;
        return *this;
    }

    Vector2 operator--(int)
    {
        Vector2 tmp(*this);
        operator--();
        return tmp;
    }

    Vector2 operator-() const
    {
        return Vector2(-x, -y);
    }

    Vector2& operator+=(const Vector2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2& operator/=(const float other)
    {
        x /= other;
        y /= other;
        return *this;
    }

    Vector2& operator*=(const float other)
    {
        x *= other;
        y *= other;
        return *this;
    }

    // Arithmatic
    friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
    friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
    friend Vector2 operator/(const Vector2& lhs, const float rhs);
    friend Vector2 operator*(const Vector2& lhs, const float rhs);

    // Comparison Operators
    friend bool operator==(const Vector2& lhs, const Vector2& rhs);
    friend bool operator!=(const Vector2& lhs, const Vector2& rhs);
    friend bool operator<(const Vector2& lhs, const Vector2& rhs);
    friend bool operator>(const Vector2& lhs, const Vector2& rhs);
    friend bool operator<=(const Vector2& lhs, const Vector2& rhs);
    friend bool operator>=(const Vector2& lhs, const Vector2& rhs);

    // Stream Operators
    friend std::ostream& operator<<(std::ostream& os, const Vector2& obj);
    friend std::istream& operator>>(std::istream& is, Vector2& obj);

    // Dot Product
    friend float operator*(const Vector2& rhs, const Vector2& lhs);

    // Normalization
    void Normalize()
    {
        *this /= Length();
    }

    Vector2 Normalized() const
    {
        return *this / Length();
    }

    // Length (Magnitude)
    float Length() const
    {
        return sqrt(*this * *this);
    }

    float LengthSquared() const
    {
        return (*this * *this);
    }

    // Distance
    float Distance(const Vector2& other) const
    {
        return (other - *this).Length();
    }

    float DistanceSquared(const Vector2& other) const
    {
        return (other - *this).LengthSquared();
    }

    // Conversion Helpers
    Vector3 ToVector3() const
    {
        return Vector3(x, y);
    }

    sf::Vector2f ToSFMLVec() const
    {
        return sf::Vector2f(x, y);
    }

    // Scalar Projection
    float ScalarProjection(const Vector2& other) const
    {
        return (*this * other) / other.Length();
    }

    // Vector Projection
    Vector2 VectorProjection(const Vector2& other) const
    {
        return other.Normalized() * ScalarProjection(other);
    }

    static Vector2 Zero()
    {
        return Vector2(0.0f, 0.0f);
    }

    // Member Variables
    float x, y;
};

inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
{
    Vector2 tmp(lhs);
    tmp += rhs;
    return tmp;
}

inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
{
    Vector2 tmp(lhs);
    tmp -= rhs;
    return tmp;
}

inline Vector2 operator/(const Vector2& lhs, const float rhs)
{
    Vector2 tmp(lhs);
    tmp /= rhs;
    return tmp;
}

inline Vector2 operator*(const Vector2& lhs, const float rhs)
{
    Vector2 tmp(lhs);
    tmp *= rhs;
    return tmp;
}

inline bool operator==(const Vector2& lhs, const Vector2& rhs) 
{
    return (fabs(lhs.x - rhs.x) < std::numeric_limits<float>::epsilon() 
            && fabs(lhs.y - rhs.y) < std::numeric_limits<float>::epsilon());
}
inline bool operator!=(const Vector2& lhs, const Vector2& rhs) { return !operator==(lhs, rhs); }
inline bool operator<(const Vector2& lhs, const Vector2& rhs) { return (lhs.x < rhs.x && lhs.y < rhs.y); }
inline bool operator>(const Vector2& lhs, const Vector2& rhs) { return  operator< (rhs, lhs); }
inline bool operator<=(const Vector2& lhs, const Vector2& rhs) { return !operator> (lhs, rhs); }
inline bool operator>=(const Vector2& lhs, const Vector2& rhs) { return !operator< (lhs, rhs); }

inline float operator*(const Vector2& lhs, const Vector2& rhs) // Dot product
{
    return (lhs.x * rhs.x + lhs.y * rhs.y);
}

inline std::ostream& operator<<(std::ostream& os, const Vector2& obj)
{
    os << "(" << obj.x << ", " << obj.y << ")";
    return os;
}

inline std::istream& operator>>(std::istream& is, Vector2& obj)
{
    is >> obj.x >> obj.y;
    return is;
}


#endif