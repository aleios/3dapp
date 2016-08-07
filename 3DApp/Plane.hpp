#ifndef PLANE_HPP
#define PLANE_HPP

#include "Vector3.hpp"

class Plane
{
public:
    void Normalize()
    {
        float mag = 1.0f / normal.Length();
        normal *= mag;
        distance *= mag;
    }

    Vector3 normal;
    float distance;
};

#endif