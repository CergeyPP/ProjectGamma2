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
	void attachTetxure(Texture* texture);

	void render();

	float FOV;

	glm::mat4 getProjectionViewMatrix();

private:

	void DisableEvent() override{}
	void EnableEvent() override {}

	float m_frameAspect;

	Texture* m_attachedTexture;
};

