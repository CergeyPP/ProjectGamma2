#pragma once
#include "Component.h"
#include "Framebuffers.h"
#include "Shader.h"

#include "Mesh.h"

class CameraComponent
	: public Component
{
public:

	CameraComponent(GameObject* gameObject);
	~CameraComponent() override;

	Texture* getRenderTexture();

	void render();

	float FOV;

	glm::mat4 getProjectionViewMatrix();

private:

	void DisableEvent() override{}
	void EnableEvent() override {}

	float m_frameAspect;

	Framebuffer m_framebuffer;
	Framebuffer m_totalFramebuffer;
	Renderbuffer m_renderbuffer;
	
	Texture* m_posTexture;
	Texture* m_normalTexture;
	Texture* m_albedoTexture;
	Texture* m_specularTexture;

	Texture* m_renderedTexture;

	Mesh* m_canvas;

	Shader* m_simple;
	Shader* m_unionShader;
};
