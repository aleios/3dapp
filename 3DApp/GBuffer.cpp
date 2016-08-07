#include "GBuffer.hpp"

/*
Format:
[Diffuse x y z][SpecularIntensity w]
[Normals x y z][SpecularPower w]
[Position x y z]
[Depth][Stencil]
*/

GBuffer::GBuffer(int inWidth, int inHeight)
: width(inWidth), height(inHeight)
{
    // Generate framebuffer.
    glGenFramebuffers(1, &fboID);

    // Generate color and normal textures.
    colorTexID = GenerateTexture(width, height, TextureType::DIFFUSE);
    normalTexID = GenerateTexture(width, height, TextureType::NORMAL);
    positionTexID = GenerateTexture(width, height, TextureType::POSITION);
    depthTexID = GenerateTexture(width, height, TextureType::DEPTH);
    finalTexID = GenerateTexture(width, height, TextureType::FINAL);

    // Bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);

    // Attach color and normal textures.
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTexID, 0);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, normalTexID, 0);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, positionTexID, 0);

    // Attach depth/stencil as texture.
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthTexID, 0);

    // Attach final results texture
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, finalTexID, 0);

    // Unbind Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GBuffer::~GBuffer()
{
    // Clean up.
    glDeleteTextures(1, &colorTexID);
    glDeleteTextures(1, &normalTexID);
    glDeleteTextures(1, &positionTexID);
    glDeleteTextures(1, &depthTexID);

    glDeleteFramebuffers(1, &fboID);
}

GLuint GBuffer::GenerateTexture(GLsizei width, GLsizei height, TextureType type)
{
    // Allocate texture.
    GLuint texID;
    glGenTextures(1, &texID);

    // Bind texture to TEXTURE_2D target.
    glBindTexture(GL_TEXTURE_2D, texID);

    GLenum internalFormat = GL_RGBA16F;
    GLenum format = GL_RGBA;
    GLenum dataType = GL_FLOAT;

    switch (type)
    {
    case TextureType::POSITION:
        internalFormat = GL_R16F;
        format = GL_RGB;
        dataType = GL_HALF_FLOAT;
        break;
    case TextureType::NORMAL:
        internalFormat = GL_RGB;
        format = GL_RGB;
        dataType = GL_UNSIGNED_BYTE;
        break;
    case TextureType::DIFFUSE:
        internalFormat = GL_RGBA;
        format = GL_RGBA;
        dataType = GL_UNSIGNED_BYTE;
        break;
    case TextureType::DEPTH:
        internalFormat = GL_DEPTH24_STENCIL8;
        format = GL_DEPTH_COMPONENT;
        dataType = GL_UNSIGNED_BYTE;
        break;
    case TextureType::FINAL:
        internalFormat = GL_RGBA16F;
        format = GL_RGBA;
        dataType = GL_HALF_FLOAT;
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, 0);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //glGenerateMipmapEXT(GL_TEXTURE_2D);

    // Storage:
    // Linear Depth: R16F
    // Normals: RGB16
    // Diffuse: RGBA16F << We want HDR rendering.
    // DepthStencil: 24/8
    // Final: RGBA16F << Still needed for HDR here...

    return texID;
}

void GBuffer::StartBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
    //glPushAttrib(GL_VIEWPORT_BIT);
    //glViewport(0, 0, width, height);

    glActiveTexture(GL_TEXTURE0);

    GLenum Buffers[] = { GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(1, Buffers);

    glClear(GL_COLOR_BUFFER_BIT);
}

void GBuffer::EndBuffer()
{
    //glPopAttrib();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::StartGeomPass()
{
    GLenum Buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, Buffers);
}

void GBuffer::StartStencilPass()
{
    glDrawBuffer(GL_NONE);
}

void GBuffer::StartLightingPass()
{
    glDrawBuffer(GL_COLOR_ATTACHMENT3);
}