#pragma once
#include "Component.h"
#include "Framebuffers.h"
#include "Shader.h"

class CameraComponent
	: public Component
{
public:

	CameraComponent(GameObject* gameObject);
	~CameraComponent() override;

	Texture* getRenderTexture();
	void attachTetxure(Texture* texture);

	void render();

	float FOV;

	glm::mat4 getProjectionViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	glm::mat4 getParticleProjectionMatrix();

private:

	void DisableEvent() override{}
	void EnableEvent() override {}

	float m_frameAspect;

	Texture* m_attachedTexture;
};

