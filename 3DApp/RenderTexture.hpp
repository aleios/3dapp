#pragma once
#ifndef RENDERTEXTURE_HPP
#define RENDERTEXTURE_HPP

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

class RenderTexture
{
public:
    RenderTexture(int inWidth, int inHeight, bool linear = false);
    ~RenderTexture();

    void Bind();
    void Unbind();

    GLuint GetTexID() const { return texID; }
private:
    int width, height;
    GLuint fboID, texID;

    GLuint GenerateTexture(GLsizei width, GLsizei height, bool linear);
};

#endif