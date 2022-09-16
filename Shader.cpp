#include "Shader.h"
#include <string>
#include <iostream>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "GLCheckError.h"

void Shader::loadVertex(const std::string shaderPath)
{
    GLint Result = GL_FALSE;
    int InfoLogLength;

    vertexShaderID_ = glCreateShader(GL_VERTEX_SHADER);

    std::string ShaderCode;
    std::ifstream ShaderStream(shaderPath, std::ios::in);
    if (ShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << ShaderStream.rdbuf();
        ShaderCode = sstr.str();
        //std::cout << VertexShaderCode << "\n";
        ShaderStream.close();
    }

    //printf("Компиляция шейдера: %s\n", shaderPath.c_str());
    const char* SourcePointer = ShaderCode.c_str();
    //std::cout << SourcePointer << std::endl;
    glShaderSource(vertexShaderID_, 1, &SourcePointer, NULL);
    glCompileShader(vertexShaderID_);

    glGetShaderiv(vertexShaderID_, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(vertexShaderID_, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(vertexShaderID_, InfoLogLength, NULL, &ShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &ShaderErrorMessage[0]);
    }
}

void Shader::loadFragment(const std::string shaderPath)
{
    GLint Result = GL_FALSE;
    int InfoLogLength;

    fragmentShaderID_ = glCreateShader(GL_FRAGMENT_SHADER);

    std::string ShaderCode;
    std::ifstream ShaderStream(shaderPath, std::ios::in);
    if (ShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << ShaderStream.rdbuf();
        ShaderCode = sstr.str();
        //std::cout << VertexShaderCode << "\n";
        ShaderStream.close();
    }

    //printf("Компиляция шейдера: %s\n", shaderPath.c_str());
    const char* SourcePointer = ShaderCode.c_str();
    //std::cout << SourcePointer << std::endl;
    glShaderSource(fragmentShaderID_, 1, &SourcePointer, NULL);
    glCompileShader(fragmentShaderID_);

    glGetShaderiv(fragmentShaderID_, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(fragmentShaderID_, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(fragmentShaderID_, InfoLogLength, NULL, &ShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &ShaderErrorMessage[0]);
    }
}

void Shader::loadGeometry(const std::string shaderPath)
{
    GLint Result = GL_FALSE;
    int InfoLogLength;

    geometryShaderID_ = glCreateShader(GL_GEOMETRY_SHADER);

    std::string ShaderCode;
    std::ifstream ShaderStream(shaderPath, std::ios::in);
    if (ShaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << ShaderStream.rdbuf();
        ShaderCode = sstr.str();
        //std::cout << VertexShaderCode << "\n";
        ShaderStream.close();
    }

    //printf("Компиляция шейдера: %s\n", shaderPath.c_str());
    const char* SourcePointer = ShaderCode.c_str();
    //std::cout << SourcePointer << std::endl;
    glShaderSource(geometryShaderID_, 1, &SourcePointer, NULL);
    glCompileShader(geometryShaderID_);

    glGetShaderiv(geometryShaderID_, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(geometryShaderID_, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(geometryShaderID_, InfoLogLength, NULL, &ShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &ShaderErrorMessage[0]);
    }
}

void Shader::linkProgram()
{
    program_ = glCreateProgram();

    GLint Result = GL_FALSE;
    int InfoLogLength;

    fprintf(stdout, "Создаем шейдерную программу и привязываем шейдеры к ней\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, vertexShaderID_);
    glAttachShader(ProgramID, fragmentShaderID_);
    if (geometryShaderID_ >= 0)
        glAttachShader(ProgramID, geometryShaderID_);
    glLinkProgram(ProgramID);

    // Проверяем шейдерную программу
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
    }

    glDeleteShader(vertexShaderID_);
    glDeleteShader(fragmentShaderID_);
    if (geometryShaderID_ >= 0)
        glDeleteShader(geometryShaderID_);

    program_ = ProgramID;

    vertexShaderID_ = -1;
    fragmentShaderID_ = -1;
    geometryShaderID_ = -1;
}

void Shader::load(const std::string& filePath)
{
    std::fstream file(filePath);
    if (file.is_open()) {

        std::string directory = filePath.substr(0, filePath.find_last_of('/') + 1);

        std::string shaderFile;

        std::cout << "Compiling " << filePath << std::endl;

        file >> shaderFile;
        loadVertex(directory + shaderFile);

        file >> shaderFile;
        loadFragment(directory + shaderFile);
        if (!file.eof()) {
            std::string shaderFile;
            file >> shaderFile;
            loadGeometry(directory + shaderFile);
        }

        linkProgram();
    }

    file.close();
}

GLuint Shader::getProgram()
{
    return program_;
}

void Shader::releaseProgram()
{
    glDeleteProgram(program_);
    program_ = -1;
}

void Shader::setUniform(const std::string name, bool value)
{
    glUseProgram(program_);
    if (uniformOffset_.find(name) == uniformOffset_.end()) {

        uniformOffset_[name] = glGetUniformLocation(program_, name.c_str());
    }
    GLuint loc = /*glGetUniformLocation(program_, name)*/uniformOffset_[name];

    glUniform1i(loc, value);
}

void Shader::setUniform(const std::string name, int value)
{
    glUseProgram(program_);
    if (uniformOffset_.find(name) == uniformOffset_.end()) {

        uniformOffset_[name] = glGetUniformLocation(program_, name.c_str());
    }
    GLuint loc = /*glGetUniformLocation(program_, name)*/uniformOffset_[name];

    glUniform1i(loc, value);
}

void Shader::setUniform(const std::string name, float value)
{
    glUseProgram(program_);
    if (uniformOffset_.find(name) == uniformOffset_.end()) {

        uniformOffset_[name] = glGetUniformLocation(program_, name.c_str());
    }
    GLuint loc = /*glGetUniformLocation(program_, name)*/uniformOffset_[name];

    glUniform1f(loc, value);
}

void Shader::setUniform(const std::string name, glm::vec3& value)
{
    glUseProgram(program_);
    if (uniformOffset_.find(name) == uniformOffset_.end()) {

        uniformOffset_[name] = glGetUniformLocation(program_, name.c_str());
    }
    GLuint loc = /*glGetUniformLocation(program_, name)*/uniformOffset_[name];

    glUniform3f(loc, value.x, value.y, value.z);
}

void Shader::setUniform(const std::string name, glm::vec2& value)
{
    glUseProgram(program_);
    if (uniformOffset_.find(name) == uniformOffset_.end()) {

        uniformOffset_[name] = glGetUniformLocation(program_, name.c_str());
    }
    GLuint loc = /*glGetUniformLocation(program_, name)*/uniformOffset_[name];

    glUniform2f(loc, value.x, value.y);
}

void Shader::setUniform(const std::string name, glm::vec4& value)
{
    glUseProgram(program_);
    if (uniformOffset_.find(name) == uniformOffset_.end()) {

        uniformOffset_[name] = glGetUniformLocation(program_, name.c_str());
    }
    GLuint loc = /*glGetUniformLocation(program_, name)*/uniformOffset_[name];

    glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void Shader::setUniform(const std::string name, glm::mat3& value)
{
    glUseProgram(program_);
    if (uniformOffset_.find(name) == uniformOffset_.end()) {

        uniformOffset_[name] = glGetUniformLocation(program_, name.c_str());
    }
    GLuint loc = /*glGetUniformLocation(program_, name)*/uniformOffset_[name];

    glUniformMatrix3fv(loc, 1, 0, glm::value_ptr(value));
}

void Shader::setUniform(const std::string name, glm::mat4& value)
{
    glUseProgram(program_);
    if (uniformOffset_.find(name) == uniformOffset_.end()) {

        uniformOffset_[name] = glGetUniformLocation(program_, name.c_str());
    }
    GLint loc = /*glGetUniformLocation(program_, name)*/uniformOffset_[name];
    //if (loc < 0) std::cout << name << " not found in shader " << program_ << std::endl;
    glUniformMatrix4fv(loc, 1, 0, glm::value_ptr(value));
}

void Shader::setTexture(const std::string name, Texture& texture)
{
    glUseProgram(program_);
    if (textureUniformOffset_.find(name) == textureUniformOffset_.end()) {

        TextureUniform uniform;
        uniform.offset = glGetUniformLocation(program_, name.c_str());
        uniform.textureIndex = textureCount;
        textureCount++;

        textureUniformOffset_[name] = uniform;
    }

    TextureUniform uniform = textureUniformOffset_[name];

    glActiveTexture(GL_TEXTURE0 + uniform.textureIndex);
    glBindTexture(texture.target(), texture.GL());
    glUniform1i(uniform.offset, uniform.textureIndex);
}