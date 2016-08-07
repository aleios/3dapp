#pragma once
#ifndef BOUNDINGSPHERE_HPP
#define BOUNDINGSPHERE_HPP

#include "Vector3.hpp"

class BoundingBox;

class BoundingSphere
{
public:
    Vector3 center;
    float radius;
};

#endif