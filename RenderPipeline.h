#pragma once
#include "Framebuffers.h"
#include "Texture.h"

#include "CameraComponent.h"
#include "StaticMeshComponent.h"
#include "LightComponent.h"

#define SHADOW_WIDTH_DIRECT 2048
#define SHADOW_HEIGHT_DIRECT 2048

#define SHADOW_WIDTH_POINT 512
#define SHADOW_HEIGHT_POINT 512

class RenderPipeline {
public:

	RenderPipeline();
	virtual ~RenderPipeline();

	/*void addDrawable(StaticMeshComponent* mesh);
	void deleteDrawable(StaticMeshComponent* mesh);

	void addLight(LightComponent* light);
	void deleteLight(LightComponent* light);*/

	virtual void drawFrom(CameraComponent* camera) = 0;
	virtual void showOnScreen() = 0;
	virtual void resize(glm::vec2 windowSize) = 0;

	virtual CameraComponent* getRenderCamera() = 0;

protected:

};
