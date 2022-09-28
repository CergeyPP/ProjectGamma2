#include "DefferedRenderPipeline.h"

#include "RenderPipeline.h"
#include "Application.h"

#include "CreateCanvasMesh.h"

DefferedRenderPipeline::DefferedRenderPipeline()
{
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

    resize(Application::get().getWindowSize());

    m_colorbuffer.attachTexture(&m_resultTexture, GL_COLOR_ATTACHMENT0);

    m_shadowShader[0] = Loader().getAsset<Shader>("Old Shaders/ShadowDirect.shader");
    m_shadowShader[1] = Loader().getAsset<Shader>("Old Shaders/Shadow.shader");
    m_shadowShader[2] = Loader().getAsset<Shader>("Old Shaders/Shadow.shader");
}

DefferedRenderPipeline::~DefferedRenderPipeline()
{
}

void DefferedRenderPipeline::drawFrom(CameraComponent* camera)
{
    glm::mat4 pv = camera->getProjectionViewMatrix();
    m_Gbuffer.clear();
    m_Gbuffer.bind();
    glCullFace(GL_FRONT);
    for (auto mesh : m_drawables) {
        mesh->drawCall(pv);
    }
    glCullFace(GL_BACK);
    m_Gbuffer.unbind();

    //light processing
    m_colorbuffer.clear();
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
    glm::mat4 projection = glm::ortho(-8.f, 8.f, -8.f, 8.f, -farplane, farplane);
    Transform transform = light->gameObject()->getGlobalTransform();
    glm::mat4 view = glm::lookAt(transform.position, transform.position + transform.forward(), glm::vec3(0, 1, 0));

    m_depthbuffer.clear();
    m_depthbuffer.bind();
    for (auto mesh : m_drawables) {
        mesh->drawCall(projection * view, m_shadowShader[0]);
    }
    m_depthbuffer.unbind();
    m_depthTexture.genMipmaps();

    m_colorbuffer.bind();
    
    m_lightShader[0]->setTexture("position", m_posTexture);
    m_lightShader[0]->setTexture("normal", m_normalTexture);
    m_lightShader[0]->setTexture("albedo", m_albedoTexture);
    m_lightShader[0]->setTexture("specular", m_specularTexture);
    m_lightShader[0]->setTexture("depthMap", m_depthTexture);
    m_lightShader[0]->setTexture("framebuffer", m_resultTexture);

    m_lightShader[0]->setUniform("farPlane", farplane);
    m_lightShader[0]->setUniform("light.diffuse", light->diffuse);
    m_lightShader[0]->setUniform("light.specular", light->specular);
    m_lightShader[0]->setUniform("light.position", glm::vec4(transform.position, 1));
    m_lightShader[0]->setUniform("light.direction", glm::vec4(transform.forward(), 1));
    m_lightShader[0]->setUniform("LightPV", projection * view);
    m_lightShader[0]->setUniform("viewPos", viewPos);

    extend::getCanvas().draw(m_lightShader[0]);
    m_colorbuffer.unbind();
}

void DefferedRenderPipeline::processLightPoint(PointLight* light, glm::vec3 viewPos)
{
    std::cout << "Process point light" << std::endl;
}

void DefferedRenderPipeline::processLightSpot(SpotLight* light, glm::vec3 viewPos)
{
    std::cout << "Process spot light" << std::endl;
}
