#include "DefferedRenderPipeline.h"

#include "RenderPipeline.h"
#include "Application.h"

#include "Model.h"

#include "CreateCanvasMesh.h"
#include "GLCheckError.h"

#include <glm/gtx/string_cast.hpp>


static glm::mat4 cameraTransform = glm::mat4(1);

DefferedRenderPipeline::DefferedRenderPipeline()
{
    glEnable(GL_MULTISAMPLE);

    m_Gbuffer.attachTexture(&m_posTexture, GL_COLOR_ATTACHMENT0);
    m_Gbuffer.attachTexture(&m_normalTexture, GL_COLOR_ATTACHMENT1);
    m_Gbuffer.attachTexture(&m_albedoTexture, GL_COLOR_ATTACHMENT2);
    m_Gbuffer.attachTexture(&m_specularTexture, GL_COLOR_ATTACHMENT3);
    m_Gbuffer.attachRenderbuffer(&m_GRenderbuffer);

    m_unionShader->setTexture("position", m_posTexture);
    m_unionShader->setTexture("normal", m_normalTexture);
    m_unionShader->setTexture("albedo", m_albedoTexture);
    m_unionShader->setTexture("specular", m_specularTexture);

    m_depthbuffer.attachTexture(&m_depthTexture, GL_COLOR_ATTACHMENT0);
    m_depthbuffer.attachRenderbuffer(&m_depthRenderbuffer);
    m_depthbuffer.setClearColor(glm::vec4(1));
    m_depthTexture.setWrapMode(GL_CLAMP_TO_BORDER);
    m_depthTexture.setBorderColor(glm::vec4(std::numeric_limits<float>::max()));

    m_shadowbuffer.attachTexture(&m_shadowMap, GL_COLOR_ATTACHMENT0);
    m_shadowbuffer.setClearColor(glm::vec4(1));
    m_shadowMap.setWrapMode(GL_CLAMP_TO_BORDER);
    m_shadowMap.setBorderColor(glm::vec4(std::numeric_limits<float>::max()));

    m_depthbufferCube.attachTexture(&m_depthTextureCube, GL_COLOR_ATTACHMENT0);
    m_depthbufferCube.attachRenderbuffer(&m_depthRenderbufferCube);
    m_depthbufferCube.setClearColor(glm::vec4(1,1,0,1));
    m_depthTextureCube.setWrapMode(GL_CLAMP_TO_BORDER);
    m_depthTextureCube.setBorderColor(glm::vec4(std::numeric_limits<float>::max()));

    resize(Application::get().getWindowSize());

    m_colorbuffer.attachTexture(&m_resultTexture, GL_COLOR_ATTACHMENT0);
    m_colorbuffer.attachRenderbuffer(&m_colorRenderbuffer);

    m_shadowShader[0] = Loader().getAsset<Shader>("Old Shaders/ShadowDirect.shader");
    m_shadowShader[1] = Loader().getAsset<Shader>("Old Shaders/Shadow.shader");
    m_shadowShader[2] = Loader().getAsset<Shader>("Old Shaders/Shadow.shader");

    m_pointLightSphere = Loader().getAsset<Model>("Sphere/simpleSphere.fbx")->getMesh(0);
}

DefferedRenderPipeline::~DefferedRenderPipeline()
{
}

void DefferedRenderPipeline::drawFrom(CameraComponent* camera)
{
    glm::mat4 pv = camera->getProjectionViewMatrix();
    m_Gbuffer.clear();
    m_Gbuffer.bind();
    for (auto mesh : m_drawables) {
        mesh->drawCall(pv);
    }
    m_Gbuffer.unbind();

    cameraTransform = pv;

    //light processing
    m_colorbuffer.clear();
    m_Gbuffer.copy(m_colorbuffer, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    for (auto light : m_lights) {
        switch (light->getType()) {
        case LightType::DIRECTIONAL:
            processLightDirect((DirectLight*)light, camera->gameObject()->getGlobalTransform().position);
            break;
        case LightType::POINT:
            processLightPoint((PointLight*)light, camera->gameObject()->getGlobalTransform().position);
            break;
        case LightType::SPOT:
            processLightSpot((SpotLight*)light, camera->gameObject()->getGlobalTransform().position);
            break;
        }
    }

    //end of light

    //m_colorbuffer.clear();
    /*m_colorbuffer.bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(m_posTexture.target(), m_posTexture.GL());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(m_normalTexture.target(), m_normalTexture.GL());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(m_albedoTexture.target(), m_albedoTexture.GL());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(m_specularTexture.target(), m_specularTexture.GL());
    extend::getCanvas().draw(m_unionShader);
    m_colorbuffer.unbind();*/
}


void DefferedRenderPipeline::showOnScreen()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(m_resultTexture.target(), m_resultTexture.GL());
    extend::getCanvas().draw(m_postProcessShader);
}

void DefferedRenderPipeline::resize(glm::vec2 windowSize)
{
    m_posTexture.resize(windowSize);
    m_normalTexture.resize(windowSize);
    m_albedoTexture.resize(windowSize);
    m_specularTexture.resize(windowSize);

    m_resultTexture.resize(windowSize);
}

void DefferedRenderPipeline::processLightDirect(DirectLight* light, glm::vec3 viewPos)
{
    float farplane = 25.f;

    //std::cout << "Process direct light" << std::endl;
    glm::mat4 projection = glm::ortho(-10.f, 10.f, -10.f, 10.f, -farplane, farplane);
    Transform transform = light->gameObject()->getGlobalTransform();
    glm::mat4 view = glm::lookAt(transform.position, transform.position + transform.forward(), glm::vec3(0, 1, 0));


    if (light->isShadowCast) {
        m_depthbuffer.clear();
        m_depthbuffer.bind();
        for (auto mesh : m_drawables) {
            mesh->drawCall(projection * view, m_shadowShader[0]);
        }
        m_depthbuffer.unbind();

        m_shadowbuffer.bind();

        glDisable(GL_DEPTH_TEST);
        m_AAShadowShader->setTexture("depthMap", m_depthTexture);
        extend::getCanvas().draw(m_AAShadowShader);

        m_shadowbuffer.unbind();

        m_shadowMap.genMipmaps();
    }
    m_colorbuffer.bind();

    m_lightShader[0]->setTexture("position", m_posTexture);
    m_lightShader[0]->setTexture("normal", m_normalTexture);
    m_lightShader[0]->setTexture("albedo", m_albedoTexture);
    m_lightShader[0]->setTexture("specular", m_specularTexture);
    m_lightShader[0]->setTexture("depthMap", m_shadowMap);
    m_lightShader[0]->setTexture("framebuffer", m_resultTexture);

    m_lightShader[0]->setUniform("farPlane", farplane);
    m_lightShader[0]->setUniform("light.diffuse", light->diffuse);
    m_lightShader[0]->setUniform("light.specular", light->specular);
    m_lightShader[0]->setUniform("light.position", glm::vec4(transform.position, 1));
    m_lightShader[0]->setUniform("light.direction", glm::vec4(transform.forward(), 1));
    m_lightShader[0]->setUniform("LightPV", projection * view);
    m_lightShader[0]->setUniform("viewPos", viewPos);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    extend::getCanvas().draw(m_lightShader[0]);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    m_colorbuffer.unbind();
}

void DefferedRenderPipeline::processLightPoint(PointLight* light, glm::vec3 viewPos)
{
    //std::cout << "Process point light" << std::endl;
    Transform lightTransform = light->gameObject()->getGlobalTransform();
    float radiusMultiplier = 1.05;
    lightTransform.scale = glm::vec3(light->radius * radiusMultiplier);

    m_depthbufferCube.clear();
    if (light->isShadowCast) {
        m_depthbufferCube.bind();

        glm::mat4 projection = glm::perspective(90.f, 1.f, 1.0f, light->radius);
        const glm::mat4 view[6] = {
            glm::lookAt(glm::vec3(lightTransform.position), glm::vec3(lightTransform.position) + glm::vec3(1,0,0), glm::vec3(0,-1,0)),
            glm::lookAt(glm::vec3(lightTransform.position), glm::vec3(lightTransform.position) + glm::vec3(-1,0,0), glm::vec3(0,-1,0)),
            glm::lookAt(glm::vec3(lightTransform.position), glm::vec3(lightTransform.position) + glm::vec3(0,1,0), glm::vec3(0,0,1)),
            glm::lookAt(glm::vec3(lightTransform.position), glm::vec3(lightTransform.position) + glm::vec3(0,-1,0), glm::vec3(0,0,-1)),
            glm::lookAt(glm::vec3(lightTransform.position), glm::vec3(lightTransform.position) + glm::vec3(0,0,1), glm::vec3(0,-1,0)),
            glm::lookAt(glm::vec3(lightTransform.position), glm::vec3(lightTransform.position) + glm::vec3(0,0,-1), glm::vec3(0,-1,0)),
        };

        for (int i = 0; i < 6; i++) {
            m_shadowShader[1]->setUniform("shadowMatrices[" + std::to_string(i) + "]", projection * view[i]);
        }
        m_shadowShader[1]->setUniform("farPlane", light->radius);
        m_shadowShader[1]->setUniform("lightPos", lightTransform.position);
        for (auto mesh : m_drawables) {
            mesh->drawCall(glm::mat4(1), m_shadowShader[1]);
        }
        m_depthbufferCube.unbind();
    }

    m_colorbuffer.bind();

    m_lightShader[1]->setTexture("position", m_posTexture);
    m_lightShader[1]->setTexture("normal", m_normalTexture);
    m_lightShader[1]->setTexture("albedo", m_albedoTexture);
    m_lightShader[1]->setTexture("specular", m_specularTexture);
    m_lightShader[1]->setTexture("depthMap", m_depthTextureCube);
    m_lightShader[1]->setTexture("framebuffer", m_resultTexture);

    m_lightShader[1]->setUniform("light.radius", light->radius);
    m_lightShader[1]->setUniform("light.diffuse", light->diffuse);
    m_lightShader[1]->setUniform("light.specular", light->specular);
    m_lightShader[1]->setUniform("light.position", glm::vec4(lightTransform.position, 1.f));
    m_lightShader[1]->setUniform("viewPos", viewPos);
    m_lightShader[1]->setUniform("light.isShadowCast", int(light->isShadowCast));

    glDisable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);
    m_pointLightSphere->draw(m_lightShader[1], cameraTransform, Transform(lightTransform.position, glm::vec3(0), lightTransform.scale));
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    //need to adaptize to use depth test

    //glDepthMask(GL_FALSE);
    //m_pointLightSphere->draw(m_lightShader[1], cameraTransform, lightTransform);
    //glDepthMask(GL_TRUE);

    m_colorbuffer.unbind();
}

void DefferedRenderPipeline::processLightSpot(SpotLight* light, glm::vec3 viewPos)
{
    std::cout << "Process spot light" << std::endl;
}
