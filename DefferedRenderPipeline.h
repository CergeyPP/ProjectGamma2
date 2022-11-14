#pragma once
#include "RenderPipeline.h"
#include "Application.h"


class DefferedRenderPipeline : public RenderPipeline
{
public:

	DefferedRenderPipeline();
	~DefferedRenderPipeline();

	void drawFrom(CameraComponent* camera) override;
	void showOnScreen() override;
	void resize(glm::vec2 windowSize) override;

private:

	void processLightDirect(DirectLight* light, glm::vec3 viewPos);
	void processLightPoint(PointLight* light, glm::vec3 viewPos);
	void processLightSpot(SpotLight* light, glm::vec3 viewPos);

	Framebuffer m_colorbuffer;
	Texture m_resultTexture = Texture(GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	Renderbuffer m_colorRenderbuffer = Renderbuffer(Application::get().getWindowSize().x, Application::get().getWindowSize().y);

	Framebuffer m_depthbuffer;
	Texture m_depthTexture = Texture(GL_TEXTURE_2D_MULTISAMPLE, GL_RGBA32F, GL_RGBA, GL_FLOAT, glm::vec2(SHADOW_WIDTH, SHADOW_HEIGHT));
	RenderbufferMultisample m_depthRenderbuffer = RenderbufferMultisample(SHADOW_WIDTH, SHADOW_HEIGHT);

	Framebuffer m_shadowbuffer;
	Texture m_shadowMap = Texture(GL_TEXTURE_2D, GL_RGBA32F, GL_RGBA, GL_FLOAT, glm::vec2(SHADOW_WIDTH, SHADOW_HEIGHT));

	Framebuffer m_depthbufferCube;
	Texture m_depthTextureCube = Texture(GL_TEXTURE_CUBE_MAP, GL_RGBA32F, GL_RGBA, GL_FLOAT, glm::vec2(SHADOW_WIDTH, SHADOW_HEIGHT));
	Renderbuffer m_depthRenderbufferCube = Renderbuffer(SHADOW_WIDTH, SHADOW_HEIGHT);

	Framebuffer m_Gbuffer;
	Texture m_posTexture = Texture(GL_TEXTURE_2D, GL_RGB16F, GL_RGBA, GL_FLOAT);
	Texture m_normalTexture = Texture(GL_TEXTURE_2D, GL_RGB16F, GL_RGBA, GL_FLOAT);
	Texture m_albedoTexture = Texture(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture m_specularTexture = Texture(GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	Renderbuffer m_GRenderbuffer = Renderbuffer(Application::get().getWindowSize().x, Application::get().getWindowSize().y);

	//Shader* m_simple;
	Shader* m_shadowShader[3];

	Shader* m_lightShader[3] = {
		Loader().getAsset<Shader>("Old Shaders/LightDirect.shader"),
		Loader().getAsset<Shader>("Old Shaders/Light.shader"),
		Loader().getAsset<Shader>("Old Shaders/Light.shader"),
	};

	Mesh* m_pointLightSphere;

	Shader* m_AAShadowShader = Loader().getAsset<Shader>("Old Shaders/AAShadow.shader");

	Shader* m_unionShader = Loader().getAsset<Shader>("UnionShader.txt");
	Shader* m_postProcessShader = Loader().getAsset<Shader>("PostProcessShader.txt");
};


