#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"

class Material {
public:

    std::map<std::string, bool> boolParam;
    std::map<std::string, int> intParam;
    std::map<std::string, float> floatParam;
    std::map<std::string, glm::vec2> vec2Param;
    std::map<std::string, glm::vec3> vec3Param;
    std::map<std::string, glm::vec4> vec4Param;
    std::map<std::string, glm::mat3> mat3Param;
    std::map<std::string, glm::mat4> mat4Param;
    std::map<std::string, Texture*> texParam;

    void use();

    Material(Shader* shader);
    ~Material();

private:

    Shader* m_shader;
};

