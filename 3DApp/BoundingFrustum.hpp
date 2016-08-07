#pragma once
#ifndef BOUNDINGFRUSTUM_HPP
#define BOUNDINGFRUSTUM_HPP

#include <array>

#include "Matrix.hpp"
#include "BoundingSphere.hpp"
#include "Plane.hpp"

class BoundingFrustum
{
public:
    BoundingFrustum(const Matrix& inMatrix)
    {
        // Planes:
        // 0 = right
        // 1 = left
        // 2 = bottom
        // 3 = top
        // 4 = far
        // 5 = near

        // Right
        planes[0].normal = Vector3(inMatrix[0][3] - inMatrix[0][0], inMatrix[1][3] - inMatrix[1][0], inMatrix[2][3] - inMatrix[2][0]);
        planes[0].distance = inMatrix[3][3] - inMatrix[3][0];

        // Left
        planes[1].normal = Vector3(inMatrix[0][3] + inMatrix[0][0], inMatrix[1][3] + inMatrix[1][0], inMatrix[2][3] + inMatrix[2][0]);
        planes[1].distance = inMatrix[3][3] + inMatrix[3][0];

        // Bottom
        planes[2].normal = Vector3(inMatrix[0][3] + inMatrix[0][1], inMatrix[1][3] + inMatrix[1][1], inMatrix[2][3] + inMatrix[2][1]);
        planes[2].distance = inMatrix[3][3] + inMatrix[3][1];

        // Top
        planes[3].normal = Vector3(inMatrix[0][3] - inMatrix[0][1], inMatrix[1][3] - inMatrix[1][1], inMatrix[2][3] - inMatrix[2][1]);
        planes[3].distance = inMatrix[3][3] - inMatrix[3][1];

        // Far
        planes[4].normal = Vector3(inMatrix[0][3] - inMatrix[0][2], inMatrix[1][3] - inMatrix[1][2], inMatrix[2][3] - inMatrix[2][2]);
        planes[4].distance = inMatrix[3][3] - inMatrix[3][2];

        // Near
        planes[5].normal = Vector3(inMatrix[0][3] + inMatrix[0][2], inMatrix[1][3] + inMatrix[1][2], inMatrix[2][3] + inMatrix[2][2]);
        planes[5].distance = inMatrix[3][3] + inMatrix[3][2];

        // Normalize plane normals.
        for (auto& plane : planes)
            plane.normal.Normalize();
    }

    bool SphereIntersection(const BoundingSphere& other)
    {
        for (auto& plane : planes)
        {
            if ((other.center * plane.normal) + plane.distance + other.radius <= 0)
                return false;
        }
        return true;
    }
private:
    std::array<Plane, 6> planes;
};

#endif