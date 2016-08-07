#pragma once
#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <sstream>
#include <fstream>

#include "VertexBufferObject.hpp"

class Model
{
public:
    void LoadFromFile(const std::string& inFilename);

    void Draw();
private:
    VertexBufferObject vbo;

    void CalculateNormals(std::vector<Vertex>& vertices, const std::vector<unsigned short>& indices);

    // Split string into tokens based on delim.
    // Evan Teran
    // Link: http://stackoverflow.com/questions/236129/how-to-split-a-string-in-c
    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }

    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }

    struct FaceDef
    {
        int pos, normal, coords;
    };
};

#endif