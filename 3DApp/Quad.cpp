#include "Quad.hpp"

Quad::Quad(float x, float y, float width, float height)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;
    Vertex v;
    v.pos = Vector3(x, y);
    v.texcoords = Vector2(1.0f, 1.0f);
    vertices.push_back(v);
    v.pos = Vector3(x + width, y);
    v.texcoords = Vector2(0.0f, 1.0f);
    vertices.push_back(v);
    v.pos = Vector3(x + width, y + height);
    v.texcoords = Vector2(0.0f, 0.0f);
    vertices.push_back(v);
    v.pos = Vector3(x, y + height);
    v.texcoords = Vector2(1.0f, 0.0f);
    vertices.push_back(v);

    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(0); indices.push_back(3); indices.push_back(2);

    vbo.SetData(vertices, indices);
}

void Quad::Render()
{
    vbo.Bind();

    glDrawElements(GL_TRIANGLES, vbo.GetIndexCount(), GL_UNSIGNED_SHORT, 0);

    vbo.Unbind();
}