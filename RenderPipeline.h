#pragma once
#include "Framebuffers.h"
#include "Texture.h"

#include "CameraComponent.h"
#include "StaticMeshComponent.h"
#include "LightComponent.h"

#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

class RenderPipeline {
public:

	RenderPipeline();
	virtual ~RenderPipeline();

	void addDrawable(StaticMeshComponent* mesh);
	void deleteDrawable(StaticMeshComponent* mesh);

	void addLight(LightComponent* light);
	void deleteLight(LightComponent* light);

	virtual void drawFrom(CameraComponent* camera) = 0;
	virtual void showOnScreen() = 0;
	virtual void resize(glm::vec2 windowSize) = 0;

protected:

	std::vector<StaticMeshComponent*> m_drawables;
	std::vector<LightComponent*> m_lights;
};
