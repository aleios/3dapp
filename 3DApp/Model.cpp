#include "Model.hpp"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <assert.h>

void Model::LoadFromFile(const std::string& inFilename)
{
    std::ifstream fs;
    fs.open(inFilename.c_str());

    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;

    std::vector<Vector3> normals;
    std::vector<Vector2> coords;
    std::vector<FaceDef> faces;

    std::cout << "Model: Reading from file (" << inFilename << ")\n";

    std::string line;
    while (std::getline(fs, line))
    {
        std::vector<std::string> tokens = split(line, ' ');

        std::string ctok = tokens[0];
        if (ctok == "v") // Define a new vertex.
        {
            Vertex v;
            v.pos.x = std::stof(tokens[1]);
            v.pos.y = std::stof(tokens[2]);
            v.pos.z = std::stof(tokens[3]);
            vertices.push_back(v);
        }
        else if (ctok == "vn") // Normals
        {
            Vector3 vec;
            vec.x = std::stof(tokens[1]);
            vec.y = std::stof(tokens[2]);
            vec.z = std::stof(tokens[3]);
            normals.push_back(vec);
        }
        else if (ctok == "vt") // Texture Coords (x y)
        {
            Vector2 vec;
            vec.x = std::stof(tokens[1]);
            vec.y = std::stof(tokens[2]);
            coords.push_back(vec);
        }
        else if (ctok == "f") // Face definition (v/vt/vn)
        {
            const int verts = 3;
            for (int i = 0; i < verts; i++)
            {
                std::vector<std::string> fdef = split(tokens[i + 1], '/');
                FaceDef f;
                f.pos = (fdef[0] == "") ? 0 : std::stoi(fdef[0]);
                if (fdef.size() > 1)
                    f.coords = (fdef[1] == "") ? 0 : std::stoi(fdef[1]);
                else
                    f.coords = 0;

                if (fdef.size() > 2)
                    f.normal = (fdef[2] == "") ? 0 : std::stoi(fdef[2]);
                else
                    f.normal = 0;

                faces.push_back(f);
            }
        }
    }

    // Done with the file so close it.
    fs.close();

    // Build up our vertex list.
    std::cout << "Model: Building vertex list.\n";

    int numNormals = 0;

    int ix = 0;

    for (auto& face : faces)
    {
        int idx = face.pos;

        // Assign vertex a normal if one exists.
        if (face.normal != 0)
        {
            vertices[idx - 1].normal = normals[face.normal - 1];
            numNormals++;
        }

        // Assign vertex texture coordinates if one exists.
        if (face.coords != 0)
            vertices[idx - 1].texcoords = coords[face.coords - 1];

        indices.push_back(idx - 1);
    }

    // No precalculated normals were found so let's calculate them here...
    if (numNormals == 0)
    {
        std::cout << "Model: No normals found. Calculating...\n";
        CalculateNormals(vertices, indices);
    }
    else
    {
        for (auto& v : vertices)
        {
            v.normal.Normalize();
        }
    }

    // Upload the vertices and indices to the GPU so we can free up memory client side.
    std::cout << "Model: Uploading to GPU.\n";

    vbo.SetData(vertices, indices);
}

void Model::CalculateNormals(std::vector<Vertex>& vertices, const std::vector<unsigned short>& indices)
{
    // Zero out the normals.
    for (auto& v : vertices)
        v.normal = Vector3();

    for (unsigned int i = 0; i < indices.size(); i+=3)
    {
        // Each iteration grab a face.
        Vertex v1 = vertices[indices[i]];
        Vertex v2 = vertices[indices[i + 1]];
        Vertex v3 = vertices[indices[i + 2]];

        Vector3 side1 = v3.pos - v1.pos;
        Vector3 side2 = v2.pos - v1.pos;
        Vector3 norm = side1.CrossProduct(side2);

        for (int v = 0; v < 3; v++)
            vertices[indices[i + v]].normal += norm;
    }

    // Normalize the normals.
    for (auto& v : vertices)
        v.normal.Normalize();
}

void Model::Draw()
{
    vbo.Bind();

    glDrawElements(GL_TRIANGLES, vbo.GetIndexCount(), GL_UNSIGNED_SHORT, 0);
}

// v = vertex x y z
// vn = normal x y z

//f = face vid/tid/nid