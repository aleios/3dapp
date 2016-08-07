#pragma once
#ifndef QUAD_HPP
#define QUAD_HPP

#include "VertexBufferObject.hpp"

class Quad
{
public:
    Quad(float x, float y, float width, float height);

    void Render();
private:
    VertexBufferObject vbo;
};

#endif