#pragma once
#ifndef GBUFFER_HPP
#define GBUFFER_HPP

/*
    == GBuffer ==
    This structure stores Position, Normals, Texcoods and lighting information.
*/

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

class GBuffer
{
public:
    enum class TextureType
    {
        POSITION,
        NORMAL,
        DIFFUSE,
        DEPTH,
        FINAL
    };

public:
    GBuffer(int inWidth, int inHeight);
    ~GBuffer();

    void StartBuffer();
    void EndBuffer();

    void StartGeomPass();
    void StartStencilPass();
    void StartLightingPass();

    GLuint GetColorID() const { return colorTexID; }
    GLuint GetNormalID() const { return normalTexID; }
    GLuint GetPositionID() const { return positionTexID; }
    GLuint GetDepthID() const { return depthTexID; }
    GLuint GetFinalID() const { return finalTexID; }
private:
    int width, height;
    GLuint fboID;
    //GLuint depthID;
    GLuint colorTexID, normalTexID, positionTexID, depthTexID, finalTexID;

    GLuint GenerateTexture(GLsizei width, GLsizei height, TextureType type);
};

#endif