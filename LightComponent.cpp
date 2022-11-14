#include "LightComponent.h"
#include "Application.h"

#include "CreateCanvasMesh.h"

LightComponent::LightComponent(GameObject* object) : Component(object)
{
	diffuse = glm::vec4(0);
	specular = glm::vec4(0);
	EnableEvent();
}

LightComponent::~LightComponent()
{
	DisableEvent();
}

void LightComponent::DisableEvent()
{
	Application::get().getPipeline().deleteLight(this);
}

void LightComponent::EnableEvent()
{
	Application::get().getPipeline().addLight(this);
}
