#include "Material.h"
#include <glm/gtc/type_ptr.hpp>

void Material::use()
{
    glUseProgram(m_shader->getProgram());

    for (auto& elem : boolParam) {
        m_shader->setUniform(elem.first, elem.second);
    }
    for (auto& elem : floatParam) {
        m_shader->setUniform(elem.first, elem.second);
    }
    for (auto& elem : intParam) {
        m_shader->setUniform(elem.first, elem.second);
    }
    for (auto& elem : vec3Param) {
        m_shader->setUniform(elem.first, elem.second);
    }
    for (auto& elem : vec4Param) {
        m_shader->setUniform(elem.first, elem.second);
    }
    for (auto& elem : vec2Param) {
        m_shader->setUniform(elem.first, elem.second);
    }
    for (auto& elem : mat3Param) {
        m_shader->setUniform(elem.first, elem.second);
    }
    for (auto& elem : mat4Param) {
        m_shader->setUniform(elem.first, elem.second);
    }
    for (auto& elem : texParam) {
        m_shader->setTexture(elem.first, *elem.second);
    }
}

Material::Material(Shader* shader) {
    m_shader = shader;
}
Material::~Material() {

}