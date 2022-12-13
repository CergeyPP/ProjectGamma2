#pragma once
#include "RenderPipeline.h"
#include "Application.h"
#include "ParticleSystem.h"

class DefferedRenderPipeline : public RenderPipeline
{
public:

	DefferedRenderPipeline();
	~DefferedRenderPipeline();

	void drawFrom(CameraComponent* camera) override;
	void showOnScreen() override;
	void resize(glm::vec2 windowSize) override;

	CameraComponent* getRenderCamera() override;

private:

	void processLightDirect(DirectLight* light, glm::vec3 viewPos);
	void processLightPoint(PointLight* light, glm::vec3 viewPos);
	void processLightSpot(SpotLight* light, glm::vec3 viewPos);


	Framebuffer m_colorbuffer;
	Texture m_resultTexture = Texture(GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	Renderbuffer m_colorRenderbuffer = Renderbuffer(Application::get().getWindowSize().x, Application::get().getWindowSize().y);

	Framebuffer m_depthbuffer;
	Texture m_depthTexture = Texture(GL_TEXTURE_2D_MULTISAMPLE, GL_RG32F, GL_RG, GL_FLOAT, glm::vec2(SHADOW_WIDTH_DIRECT, SHADOW_HEIGHT_DIRECT));
	RenderbufferMultisample m_depthRenderbuffer = RenderbufferMultisample(SHADOW_WIDTH_DIRECT, SHADOW_HEIGHT_DIRECT);

	Framebuffer m_shadowbuffer;
	Texture m_shadowMap = Texture(GL_TEXTURE_2D, GL_RG32F, GL_RG, GL_FLOAT, glm::vec2(SHADOW_WIDTH_DIRECT, SHADOW_HEIGHT_DIRECT));

	Framebuffer m_depthbufferCube;
	Texture m_depthTextureCube = Texture(GL_TEXTURE_CUBE_MAP, GL_RG32F, GL_RG, GL_FLOAT, glm::vec2(SHADOW_WIDTH_POINT, SHADOW_HEIGHT_POINT));
	Renderbuffer m_depthRenderbufferCube = Renderbuffer(SHADOW_WIDTH_POINT, SHADOW_HEIGHT_POINT);

	Framebuffer m_Gbuffer;
	Texture m_posTexture = Texture(GL_TEXTURE_2D, GL_RGB16F, GL_RGBA, GL_FLOAT);
	Texture m_normalTexture = Texture(GL_TEXTURE_2D, GL_RGB16F, GL_RGBA, GL_FLOAT);
	Texture m_albedoTexture = Texture(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture m_specularTexture = Texture(GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	Renderbuffer m_GRenderbuffer = Renderbuffer(Application::get().getWindowSize().x, Application::get().getWindowSize().y);

	CameraComponent* m_renderCamera;
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

	ComponentService<StaticMeshComponent> m_staticMeshes;
	ComponentService<DirectLight> m_directLights;
	ComponentService<PointLight> m_pointLights;
	ComponentService<SpotLight> m_spotLights;
	ComponentService<ParticleSystem> m_particleSystems;
};


