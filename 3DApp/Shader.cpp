#include "Shader.hpp"
#include <fstream>
#include <iostream>

#include <array>

Shader::Shader()
    : programID(0), vertID(0), fragID(0)
{
}

Shader::~Shader()
{
    glDeleteShader(vertID);
    glDeleteShader(fragID);
    glDeleteProgram(programID);
}

void Shader::LoadFromFile(const std::string& inVertFilename, const std::string& inFragFilename)
{
    std::cout << "Shader: Loading from (" << inVertFilename << ", " << inFragFilename << ")\n";
    // Allocate the program.
    programID = glCreateProgram();

    // Allocate the shaders.
    vertID = glCreateShader(GL_VERTEX_SHADER);
    fragID = glCreateShader(GL_FRAGMENT_SHADER);

    // Load the vertex shader.
    std::string vertStr = ReadShader(inVertFilename);
    const char* vertSource = vertStr.c_str();

    glShaderSource(vertID, 1, &vertSource, 0);
    glCompileShader(vertID);
    glAttachShader(programID, vertID);

    GLint success = GL_FALSE;
    glGetShaderiv(vertID, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        char vertLog[2048];
        glGetShaderInfoLog(vertID, 2048, NULL, vertLog);

        std::cout << "Vertex Shader Compile Log: " << vertLog << "\n";
        return; // Should throw an exception.
    }

    // Load the fragment shader.
    std::string fragStr = ReadShader(inFragFilename);
    const char* fragSource = fragStr.c_str();

    glShaderSource(fragID, 1, &fragSource, 0);
    glCompileShader(fragID);
    glAttachShader(programID, fragID);

    glGetShaderiv(fragID, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        char fragLog[2048];
        glGetShaderInfoLog(fragID, 2048, NULL, fragLog);

        std::cout << "Fragment Shader Compile Log: " << fragLog << "\n";
        return; // Should throw an exception.
    }

    // Link the program.
    glLinkProgram(programID);

    // Check the status on the linking.
    glGetProgramiv(programID, GL_LINK_STATUS, &success);

    // Output failure and throw if necessary.
    if (success != GL_TRUE)
    {
#ifdef _DEBUG
        char progLog[2048];
        glGetProgramInfoLog(programID, 2048, 0, progLog);
        std::cout << "Shader Linker Log: " << progLog << "\n";
#endif
    }
}

void Shader::Bind(Shader* inShader)
{
    glUseProgram((inShader == nullptr) ? 0 : inShader->GetProgramID());
}

std::string Shader::ReadShader(const std::string& inFilename) const
{
    std::ifstream file(inFilename);
    std::string shader = "";

    std::string line = "";
    while (std::getline(file, line))
    {
        shader += line + "\n";
    }
    return shader;
}

GLuint Shader::GetParameterLocation(const std::string& inName) const
{
    return glGetUniformLocation(programID, inName.c_str());
}

GLuint Shader::GetAttributeLocation(const std::string& inName) const
{
    return glGetAttribLocation(programID, inName.c_str());
}

void Shader::SetParameter(const std::string& inName, GLuint id)
{
    GLint loc = glGetUniformLocation(programID, inName.c_str());
    glUniform1ui(loc, id);
}

void Shader::SetParameter(const std::string& inName, GLint id)
{
    GLint loc = glGetUniformLocation(programID, inName.c_str());
    glUniform1i(loc, id);
}

void Shader::SetParameter(const std::string& inName, float x)
{
    GLint loc = glGetUniformLocation(programID, inName.c_str());
    glUniform1f(loc, x);
}

void Shader::SetParameter(const std::string& inName, float x, float y)
{
    GLint loc = glGetUniformLocation(programID, inName.c_str());
    glUniform2f(loc, x, y);
}

void Shader::SetParameter(const std::string& inName, float x, float y, float z)
{
    GLint loc = glGetUniformLocation(programID, inName.c_str());
    glUniform3f(loc, x, y, z);
}

void Shader::SetParameter(const std::string& inName, float x, float y, float z, float w)
{
    GLint loc = glGetUniformLocation(programID, inName.c_str());
    glUniform4f(loc, x, y, z, w);
}

void Shader::SetParameter(const std::string& inName, float* fv)
{
    GLint loc = glGetUniformLocation(programID, inName.c_str());

    glUniformMatrix4fv(loc, 1, GL_FALSE, fv);
}