#pragma once
#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "AEmath.hpp"
#include "Color.hpp"

class Vertex
{
public:
    Vertex()
        : pos(0.0f, 0.0f, 0.0f), normal(1.0f, 1.0f, 1.0f), tangent(1.0f, 1.0f, 1.0f), bitangent(0.0f, 0.0f, 0.0f), texcoords(0.0f, 0.0f), col(255, 255, 255, 255)
    {
    }

    Vertex(const Vector3& pos, const Color& col)
        : pos(pos), col(col), normal(1, 1, 1)
    {
    }

    Vector3 pos, normal, tangent, bitangent;
    Vector2 texcoords;
    Color col;

    inline friend bool operator==(const Vertex& lhs, const Vertex& rhs);
    inline friend bool operator!=(const Vertex& lhs, const Vertex& rhs);
};

bool operator==(const Vertex& lhs, const Vertex& rhs)
{
    return lhs.pos == rhs.pos && lhs.normal == rhs.normal
        && lhs.tangent == rhs.tangent && lhs.bitangent == rhs.bitangent
        && lhs.texcoords == rhs.texcoords && lhs.col == rhs.col;
}

bool operator!=(const Vertex& lhs, const Vertex& rhs)
{
    return !(lhs == rhs);
}

#endif