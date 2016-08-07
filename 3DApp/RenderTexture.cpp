#include "RenderTexture.hpp"
#include <iostream>

RenderTexture::RenderTexture(int inWidth, int inHeight, bool linear)
: width(inWidth), height(inHeight)
{
    // Generate framebuffer.
    glGenFramebuffers(1, &fboID);

    texID = GenerateTexture(inWidth, inHeight, linear);

    // Bind framebuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);

    // Attach texture.
    glBindTexture(GL_TEXTURE_2D, texID);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texID, 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "RenderTexture: Invalid FBO. Reason: \n";

        if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
            std::cout << "Attachment incomplete.\n";
        else if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
            std::cout << "Missing attachment.\n";
        else if (status == GL_FRAMEBUFFER_UNSUPPORTED)
            std::cout << "Format unsupported.\n";
    }

    // Unbind framebuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderTexture::~RenderTexture()
{
    glDeleteTextures(1, &texID);
    glDeleteFramebuffers(1, &fboID);
}

GLuint RenderTexture::GenerateTexture(GLsizei width, GLsizei height, bool linear)
{
    // Allocate texture.
    GLuint texID;
    glGenTextures(1, &texID);

    // Bind texture to TEXTURE_2D target.
    glBindTexture(GL_TEXTURE_2D, texID);

    GLenum internalFormat = GL_RGB16F;
    GLenum format = GL_RGB;
    GLenum dataType = GL_FLOAT;

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, 0);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (linear) ? GL_LINEAR: GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (linear) ? GL_LINEAR : GL_NEAREST);

    return texID;
}

void RenderTexture::Bind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboID);

    glPushAttrib(GL_VIEWPORT);
    glViewport(0, 0, width, height);
    glActiveTexture(GL_TEXTURE0);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderTexture::Unbind()
{
    glPopAttrib();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}