#include "VertexBufferObject.hpp"

VertexBufferObject::VertexBufferObject()
: isInit(false)
{
    glGenVertexArrays(1, &vaoID);
    glGenBuffers(2, buffers);
}


VertexBufferObject::~VertexBufferObject()
{
    glDeleteBuffers(2, buffers);
    glDeleteVertexArrays(1, &vaoID);
}


void VertexBufferObject::Bind()
{
    glBindVertexArray(vaoID);
}

void VertexBufferObject::Unbind()
{
    glBindVertexArray(0);
}

void VertexBufferObject::SetData(const std::vector<Vertex>& inVertices, const std::vector<unsigned short>& inIndices, GLenum usage, bool orphan)
{
    vertexCount = inVertices.size();
    indexCount = inIndices.size();
    if (vertexCount < 1)
        return;

    // Bind the Vertex Array Object
    SafeBind();

    // Bind the Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    if (!isInit)
    {
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), &inVertices.front(), usage);
    }
    else
    {
        if (orphan) glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), 0, usage);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * sizeof(Vertex), &inVertices.front());
    }

    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos.x));
    glEnableVertexAttribArray(0);

    // Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal.x));
    glEnableVertexAttribArray(1);

    // Tangents
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tangent.x));
    glEnableVertexAttribArray(2);

    // Bitangents
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bitangent.x));
    glEnableVertexAttribArray(3);

    // TexCoords
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoords.x));
    glEnableVertexAttribArray(4);

    // Colors
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, col.r));
    glEnableVertexAttribArray(5);

    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    if (!isInit)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned short), &inIndices.front(), usage);
    else
    {
        if (orphan) glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned short), 0, usage);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexCount * sizeof(unsigned short), &inIndices.front());
    }

    SafeUnbind();

    isInit = true;
}

void VertexBufferObject::ReplaceVertexData(const std::vector<Vertex>& vertices, GLintptr offset)
{
    SafeBind();
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferSubData(GL_ARRAY_BUFFER, offset, vertices.size() * sizeof(Vertex), &vertices.front());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    SafeUnbind();
}

void VertexBufferObject::SafeBind()
{
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevBound);
    if ((GLuint)prevBound != vaoID)
        glBindVertexArray(vaoID);
}

void VertexBufferObject::SafeUnbind()
{
    if ((GLuint)prevBound != vaoID)
        glBindVertexArray(vaoID);
    else
        glBindVertexArray(0);
}