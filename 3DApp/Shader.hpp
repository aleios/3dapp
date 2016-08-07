#pragma once
#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <string>

class Shader
    : private sf::NonCopyable
{
public:
    Shader();
    ~Shader();
    void LoadFromFile(const std::string& inVertFilename, const std::string& inFragFilename);

    GLuint GetProgramID() const { return programID; }

    static void Bind(Shader* inShader = nullptr);

    // Parameters and Attributes
    GLuint GetParameterLocation(const std::string& inName) const;
    GLuint GetAttributeLocation(const std::string& inName) const;

    void SetParameter(const std::string& inName, GLuint id);
    void SetParameter(const std::string& inName, GLint id);
    void SetParameter(const std::string& inName, float x);
    void SetParameter(const std::string& inName, float x, float y);
    void SetParameter(const std::string& inName, float x, float y, float z);
    void SetParameter(const std::string& inName, float x, float y, float z, float w);
    void SetParameter(const std::string& inName, float* fv);
private:
    GLuint programID, vertID, fragID;

    std::string Shader::ReadShader(const std::string& inFilename) const;
};

#endif