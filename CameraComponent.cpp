#include "CameraComponent.h"
#include "Application.h"

#include <glm/gtx/string_cast.hpp>

#include "CreateCanvasMesh.h"

CameraComponent::CameraComponent(GameObject* gameObject) : Component(gameObject),
    m_renderbuffer(Application::get().getWindowSize().x, Application::get().getWindowSize().y)
{

    m_simple = new Shader;
    m_simple->load("Assets/ShaderSimple.txt");

    m_unionShader = new Shader;
    m_unionShader->load("Assets/UnionShader.txt");

    FOV = 90.f;
    m_frameAspect = 16.f / 9.f;
    Application::get().scene().addCamera(this);

    m_canvas = extend::createCanvas();

    m_posTexture =  new Texture(GL_TEXTURE_2D, GL_RGB16F, GL_RGBA, GL_FLOAT, glm::vec2(Application::get().getWindowSize()));
    m_normalTexture = new Texture(GL_TEXTURE_2D, GL_RGB16F, GL_RGBA, GL_FLOAT, glm::vec2(Application::get().getWindowSize()));
    m_albedoTexture = new Texture(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, glm::vec2(Application::get().getWindowSize()));
    m_specularTexture = new Texture(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, glm::vec2(Application::get().getWindowSize()));

    m_framebuffer.attachTexture(m_posTexture, GL_COLOR_ATTACHMENT0);
    m_framebuffer.attachTexture(m_normalTexture, GL_COLOR_ATTACHMENT1);
    m_framebuffer.attachTexture(m_albedoTexture, GL_COLOR_ATTACHMENT2);
    m_framebuffer.attachTexture(m_specularTexture, GL_COLOR_ATTACHMENT3);

    std::cout << "T1:" << std::endl;
    m_unionShader->setTexture("position", *m_posTexture);
    std::cout << "T2:" << std::endl;
    m_unionShader->setTexture("normal", *m_normalTexture);
    std::cout << "T3:" << std::endl;
    m_unionShader->setTexture("albedo", *m_albedoTexture);
    std::cout << "T4:" << std::endl;
    m_unionShader->setTexture("specular", *m_specularTexture);

    m_framebuffer.attachRenderbuffer(&m_renderbuffer);
}

CameraComponent::~CameraComponent()
{
    Application::get().scene().deleteCamera(this);
}

Texture* CameraComponent::getRenderTexture(){
    return &(m_renderedTexture);
}

glm::mat4 CameraComponent::getProjectionViewMatrix()
{
    glm::mat4 proj = glm::perspective(FOV / 2.f, m_frameAspect, 0.1f, 100.f);

    glm::mat4 view = glm::lookAt(gameObject()->getGlobalTransform().position,
        /*gameObject()->getGlobalTransform().position + gameObject()->getGlobalTransform().forward(),*/glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0));

    return proj * view;
}

void CameraComponent::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 pv = getProjectionViewMatrix();
    m_framebuffer.clear();
    m_framebuffer.bind();
    Application::get().scene().drawCallEvent(pv, m_simple);
    m_framebuffer.unbind();
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glViewport(0, 0, Application::get().getWindowSize().x, Application::get().getWindowSize().y);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(m_posTexture->target(), m_posTexture->GL());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(m_normalTexture->target(), m_normalTexture->GL());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(m_albedoTexture->target(), m_albedoTexture->GL());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(m_specularTexture->target(), m_specularTexture->GL());
    m_canvas->draw(m_unionShader);
    
}