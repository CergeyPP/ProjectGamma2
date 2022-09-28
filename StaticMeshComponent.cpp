#include "StaticMeshComponent.h"
#include "Application.h"

StaticMeshComponent::StaticMeshComponent(GameObject* parent)
	: Component(parent)
{
	mesh = nullptr;
	EnableEvent();
}

StaticMeshComponent::~StaticMeshComponent()
{
	DisableEvent();
}

void StaticMeshComponent::drawCall(const glm::mat4& ProjectionView, Shader* shader)
{
	if (mesh == nullptr) return;
	mesh->draw(shader, ProjectionView, gameObject()->getGlobalTransform());
}

void StaticMeshComponent::DisableEvent()
{
	Application::get().getPipeline().deleteDrawable(this);
}

void StaticMeshComponent::EnableEvent()
{
	Application::get().getPipeline().addDrawable(this);
}
