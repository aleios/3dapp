#pragma once
#ifndef VERTEXBUFFEROBJECT_HPP
#define VERTEXBUFFEROBJECT_HPP

#include <vector>

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include "Vertex.hpp"

class VertexBufferObject
{
public:
    VertexBufferObject();
    ~VertexBufferObject();

    // Disable copy
    VertexBufferObject(const VertexBufferObject& other) = delete;
    VertexBufferObject& operator=(const VertexBufferObject& other) = delete;

    void Bind();
    void Unbind();

    void SetData(const std::vector<Vertex>& inVertices, const std::vector<unsigned short>& inIndices, GLenum usage = GL_STATIC_DRAW, bool inOrphan = false);
    void ReplaceVertexData(const std::vector<Vertex>& inVertices, GLintptr inOffset);
    void ReplaceIndexData(const std::vector<unsigned short>& inIndices, GLintptr inOffset);

    GLsizei GetVertexCount() const { return vertexCount; }
    GLsizei GetIndexCount() const { return indexCount; }
private:
    GLuint vaoID;
    GLuint buffers[2];
    GLsizei vertexCount{ 0 }, indexCount{ 0 };

    void SafeBind();
    void SafeUnbind();
    GLint prevBound;
    bool isInit;
};

#endif