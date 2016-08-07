#pragma once
#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <cmath>

#include "Vector3.hpp"

class Quaternion
{
public:
    Quaternion(float X = 0.0f, float Y = 0.0f, float Z = 0.0f, float W = 1.0f)
        : x(X), y(Y), z(Z), w(W)
    {
    }

    Quaternion(const Vector3& vec, float scalar)
        : w(scalar)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
    }

    const Quaternion& operator*=(const Quaternion& other)
    {
        w = (w * other.w - x * other.x - y * other.y - z * other.z);
        x = (w * other.x + x * other.w + y * other.z - z * other.y);
        y = (w * other.y - x * other.z + y * other.w + z * other.x);
        z = (w * other.z + x * other.y - y * other.x + z * other.w);

        return *this;
    }

    // Multiply and Transform
    friend Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);

    // Divide
    Quaternion operator/(float scalar) const
    {
        return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
    }

    // Dot
    float Dot(const Quaternion& quat)
    {
        return x * quat.x + y * quat.y + z * quat.z + w * quat.w;
    }

    static float Dot(const Quaternion& q1, const Quaternion& q2)
    {
        return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
    }

    // Inverse
    const Quaternion& Inverse()
    {
        x = w;
        y = -x;
        z = -y;
        w = -z;
        *this / Dot(*this);

        return *this;
    }

    static Quaternion Inverse(const Quaternion& quat)
    {
        return Quaternion(quat.w, -quat.x, -quat.y, -quat.z) / Dot(quat, quat);
    }

    // Length
    float Length() const
    {
        return sqrt(w * w + x * x + y * y + z * z);
    }

    float LengthSquared() const
    {
        return w * w + x * x + y * y + z * z;
    }

    // Normalization
    Quaternion Normalized() const
    {
        Quaternion res(*this);
        float mag = Length();

        res.w /= mag;
        res.x /= mag;
        res.y /= mag;
        res.z /= mag;

        return res;
    }

    void Normalize()
    {
        float mag = Length();
        w /= mag;
        x /= mag;
        y /= mag;
        z /= mag;
    }

    // Identity
    static Quaternion Identity()
    {
        return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    }

    // Yaw, Pitch, roll
    static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll)
    {
        Quaternion quat;

        float cy = cos(yaw * 0.5f), cp = cos(pitch * 0.5f), cr = cos(roll * 0.5f);
        float sy = sin(yaw * 0.5f), sp = sin(pitch * 0.5f), sr = sin(roll * 0.5f);

        quat.x = ((cy * sp) * cr) + ((sy * cp) * sr);
        quat.y = ((sy * cp) * cr) - ((cy * sp) * sr);
        quat.z = ((cy * cp) * sr) - ((sy * sp) * cr);
        quat.w = ((cy * cp) * cr) + ((sy * sp) * sr);
        return quat;
    }

    static Quaternion CreateFromAxisAngle(const Vector3& vec, float angle)
    {
        Quaternion quat;

        angle *= 0.5f;
        Vector3 norm = vec.Normalized();
 
        float s = sin(angle);
        float c = cos(angle);
 
        quat.x = (norm.x * s);
        quat.y = (norm.y * s);
        quat.z = (norm.z * s);
        quat.w = c;

        quat.Normalize();
        return quat;
    }

    // Vector3 Transform
    static Vector3 GetVectorTransform(const Quaternion& quat, const Vector3& vec)
    {
        Vector3 qvec(quat.x, quat.y, quat.z);
        Vector3 v1 = qvec.CrossProduct(vec);
        Vector3 v2 = qvec.CrossProduct(v1);

        v1 *= (2 * quat.w);
        v2 *= 2;

        return vec + v1 + v2;
    }

    float x, y, z, w;
};

inline Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
{
    Quaternion tmp(lhs);
    tmp *= rhs;
    return tmp;
}

#endif