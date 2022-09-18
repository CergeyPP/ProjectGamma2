#include "LightComponent.h"
#include "Application.h"

#include "CreateCanvasMesh.h"

LightComponent::LightComponent(GameObject* object) : Component(object)
{
	EnableEvent();
	m_lightPassShader = Loader().getAsset<Shader>("Light.shader");
	m_depthPassShader = Loader().getAsset<Shader>("Shadow.shader");
	m_directLightPassShader = Loader().getAsset<Shader>("LightDirect.shader");
	m_directDepthPassShader = Loader().getAsset<Shader>("ShadowDirect.shader");
}

LightComponent::~LightComponent()
{
	DisableEvent();
}

void LightComponent::lightProcess(Framebuffer& sourceBuffer, Framebuffer& colorFramebuffer, glm::vec3 viewPos)
{
	Transform transform = gameObject()->getGlobalTransform();
	glm::vec4 pos = glm::vec4(transform.position, 1.f);
	glm::vec4 direction = glm::vec4(transform.forward(), 1.f);
	//here is shadow pass

	Framebuffer& depthFramebuffer = type == LightType::DIRECTIONAL ? Application::get().getDepthFramebuffer2D() : Application::get().getDepthFramebuffer();
	depthFramebuffer.clear();
	depthFramebuffer.bind();

	float nearplane = 0.1f;
	float farplane = 100.f;

	glm::mat4 pv = glm::mat4(1);
	

	if (type == LightType::DIRECTIONAL) {
		glm::mat4 lightProjection = glm::ortho(-10.f, 10.f, -10.f, 10.f, nearplane, farplane);
		glm::mat4 lightView = glm::lookAt(glm::vec3(pos), glm::vec3(pos) + glm::vec3(direction), glm::vec3(0, 1, 0));

		m_directDepthPassShader->setUniform("LightPV", lightProjection * lightView);
		glCullFace(GL_FRONT);
		MainScene().drawCallEvent(pv, m_directDepthPassShader);
		glCullFace(GL_BACK);
		depthFramebuffer.unbind();

		//end of shadow pass

		colorFramebuffer.bind();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);

		m_directLightPassShader->setTexture("position", *sourceBuffer.getTextureByAttachment(GL_COLOR_ATTACHMENT0));
		m_directLightPassShader->setTexture("normal", *sourceBuffer.getTextureByAttachment(GL_COLOR_ATTACHMENT1));
		m_directLightPassShader->setTexture("albedo", *sourceBuffer.getTextureByAttachment(GL_COLOR_ATTACHMENT2));
		m_directLightPassShader->setTexture("specular", *sourceBuffer.getTextureByAttachment(GL_COLOR_ATTACHMENT3));

		m_directLightPassShader->setTexture("depthMap", Application::get().getDepthTexture2D());

		m_directLightPassShader->setTexture("framebuffer", *colorFramebuffer.getTextureByAttachment(GL_COLOR_ATTACHMENT0));

		m_directLightPassShader->setUniform("light.type", (int)type);
		m_directLightPassShader->setUniform("light.cutoff", cutoff);
		m_directLightPassShader->setUniform("light.linear", linear);
		m_directLightPassShader->setUniform("light.quadratic", quadratic);
		m_directLightPassShader->setUniform("light.diffuse", diffuse);
		m_directLightPassShader->setUniform("light.specular", specular);

		m_directLightPassShader->setUniform("LightPV", lightProjection * lightView);

		m_directLightPassShader->setUniform("farPlane", farplane);

		m_directLightPassShader->setUniform("light.position", pos);
		m_directLightPassShader->setUniform("light.direction", direction);

		m_directLightPassShader->setUniform("viewPos", viewPos);

		extend::getCanvas().draw(m_directLightPassShader);

		colorFramebuffer.unbind();
	}
	else {
		glm::mat4 lightProjection = glm::perspective(glm::radians(90.f), 1.f, nearplane, farplane);

		const std::vector<glm::mat4> lightView = {
				glm::lookAt(glm::vec3(pos), glm::vec3(pos) + glm::vec3(1,0,0), glm::vec3(0,-1,0)),
				glm::lookAt(glm::vec3(pos), glm::vec3(pos) + glm::vec3(-1,0,0), glm::vec3(0,-1,0)),
				glm::lookAt(glm::vec3(pos), glm::vec3(pos) + glm::vec3(0,1,0), glm::vec3(0,0,1)),
				glm::lookAt(glm::vec3(pos), glm::vec3(pos) + glm::vec3(0,-1,0), glm::vec3(0,0,-1)),
				glm::lookAt(glm::vec3(pos), glm::vec3(pos) + glm::vec3(0,0,1), glm::vec3(0,-1,0)),
				glm::lookAt(glm::vec3(pos), glm::vec3(pos) + glm::vec3(0,0,-1), glm::vec3(0,-1,0)),
		};

		for (int i = 0; i < 6; i++) {
			m_depthPassShader->setUniform("shadowMatrices[" + std::to_string(i) + "]", lightProjection * lightView[i]);
		}

		m_depthPassShader->setUniform("farPlane", farplane);
		m_depthPassShader->setUniform("light.type", (int)type);
		m_depthPassShader->setUniform("light.position", pos);

		glCullFace(GL_FRONT);
		MainScene().drawCallEvent(pv, m_depthPassShader);
		glCullFace(GL_BACK);
		depthFramebuffer.unbind();

		//end of shadow pass

		colorFramebuffer.bind();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);

		m_lightPassShader->setTexture("position", *sourceBuffer.getTextureByAttachment(GL_COLOR_ATTACHMENT0));
		m_lightPassShader->setTexture("normal", *sourceBuffer.getTextureByAttachment(GL_COLOR_ATTACHMENT1));
		m_lightPassShader->setTexture("albedo", *sourceBuffer.getTextureByAttachment(GL_COLOR_ATTACHMENT2));
		m_lightPassShader->setTexture("specular", *sourceBuffer.getTextureByAttachment(GL_COLOR_ATTACHMENT3));

		m_lightPassShader->setTexture("depthMap", Application::get().getDepthTexture());

		m_lightPassShader->setTexture("framebuffer", *colorFramebuffer.getTextureByAttachment(GL_COLOR_ATTACHMENT0));

		m_lightPassShader->setUniform("light.type", (int)type);
		m_lightPassShader->setUniform("light.cutoff", cutoff);
		m_lightPassShader->setUniform("light.linear", linear);
		m_lightPassShader->setUniform("light.quadratic", quadratic);
		m_lightPassShader->setUniform("light.diffuse", diffuse);
		m_lightPassShader->setUniform("light.specular", specular);

		m_lightPassShader->setUniform("farPlane", farplane);

		m_lightPassShader->setUniform("light.position", pos);
		m_lightPassShader->setUniform("light.direction", direction);

		m_lightPassShader->setUniform("viewPos", viewPos);

		extend::getCanvas().draw(m_lightPassShader);

		colorFramebuffer.unbind();
	}
	
}

void LightComponent::DisableEvent()
{
	MainScene().lightPassEvent -= MY_METHOD_HANDLER(LightComponent::lightProcess);
}

void LightComponent::EnableEvent()
{
	MainScene().lightPassEvent += MY_METHOD_HANDLER(LightComponent::lightProcess);
}
