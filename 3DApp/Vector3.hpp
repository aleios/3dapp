#pragma once
#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <cmath>
#include <iostream>

#include <SFML/System/Vector3.hpp>

class Vector3
{
public:
    Vector3(float inX = 0.0f, float inY = 0.0f, float inZ = 0.0f)
        : x(inX), y(inY), z(inZ)
    {
    }

    Vector3(const Vector3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }

    ~Vector3()
    {
    }

    // Assignment
    Vector3& operator=(const Vector3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    // Arithmatic Operators
    Vector3& operator++()
    {
        x += 1.0f;
        y += 1.0f;
        z += 1.0f;
        return *this;
    }

    Vector3 operator++(int)
    {
        Vector3 tmp(*this);
        operator++();
        return tmp;
    }

    Vector3& operator--()
    {
        x -= 1.0f;
        y -= 1.0f;
        z -= 1.0f;
        return *this;
    }

    Vector3 operator--(int)
    {
        Vector3 tmp(*this);
        operator--();
        return tmp;
    }

    Vector3 operator-() const
    {
        return Vector3(-x, -y, -z);
    }

    const Vector3& operator+=(const Vector3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    const Vector3& operator-=(const Vector3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    const Vector3& operator/=(const float scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    const Vector3& operator*=(const float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    // Arithmatic
    friend Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
    friend Vector3 operator-(const Vector3& lhs, const Vector3& rhs);
    friend Vector3 operator/(const Vector3& lhs, const float scalar);
    friend Vector3 operator*(const Vector3& lhs, const float scalar);

    // Comparison Operators
    friend bool operator==(const Vector3& lhs, const Vector3& rhs);
    friend bool operator!=(const Vector3& lhs, const Vector3& rhs);
    friend bool operator<(const Vector3& lhs, const Vector3& rhs);
    friend bool operator>(const Vector3& lhs, const Vector3& rhs);
    friend bool operator<=(const Vector3& lhs, const Vector3& rhs);
    friend bool operator>=(const Vector3& lhs, const Vector3& rhs);

    // Stream Operators
    friend std::ostream& operator<<(std::ostream& os, const Vector3& obj);
    friend std::istream& operator>>(std::istream& is, Vector3& obj);

    // Dot Product
    float operator*(const Vector3& other) const
    {
        return (x * other.x + y * other.y + z * other.z);
    }

    // Cross Product
    Vector3 CrossProduct(const Vector3& other) const
    {
        return Vector3((y * other.z) - (z * other.y), -((x * other.z) - (z * other.x)), (x * other.y) - (y * other.x));
    }

    // Length
    float LengthSquared() const
    {
        return (*this * *this);
    }

    float Length() const
    {
        return sqrt(LengthSquared());
    }

    // Normalization
    void Normalize()
    {
        *this /= Length();
    }

    Vector3 Normalized() const
    {
        return *this / Length();
    }

    // Distance
    float DistanceSquared(const Vector3& other) const
    {
        return (other - *this).LengthSquared();
    }

    float Distance(const Vector3& other) const
    {
        return (other - *this).Length();
    }

    // Conversions
    sf::Vector3f ToSFMLVec() const
    {
        return sf::Vector3f(x, y, z);
    }

    // Member Variables
    float x, y, z;
private:
};

inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
{
    Vector3 tmp(lhs);
    tmp += rhs;
    return tmp;
}

inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
{
    Vector3 tmp(lhs);
    tmp -= rhs;
    return tmp;
}

inline Vector3 operator/(const Vector3& lhs, const float scalar)
{
    Vector3 tmp(lhs);
    tmp /= scalar;
    return tmp;
}

inline Vector3 operator*(const Vector3& lhs, const float scalar)
{
    Vector3 tmp(lhs);
    tmp *= scalar;
    return tmp;
}

inline bool operator==(const Vector3& lhs, const Vector3& rhs) { return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z); }
inline bool operator!=(const Vector3& lhs, const Vector3& rhs) { return !operator==(lhs, rhs); }
inline bool operator<(const Vector3& lhs, const Vector3& rhs) { return (lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z); }
inline bool operator>(const Vector3& lhs, const Vector3& rhs) { return  operator< (rhs, lhs); }
inline bool operator<=(const Vector3& lhs, const Vector3& rhs) { return !operator> (lhs, rhs); }
inline bool operator>=(const Vector3& lhs, const Vector3& rhs) { return !operator< (lhs, rhs); }

inline std::ostream& operator<<(std::ostream& os, const Vector3& obj)
{
    os << "(" << obj.x << ", " << obj.y << ", " << obj.z << ")";
    return os;
}

inline std::istream& operator>>(std::istream& is, Vector3& obj)
{
    is >> obj.x >> obj.y >> obj.z;
    return is;
}

#endif