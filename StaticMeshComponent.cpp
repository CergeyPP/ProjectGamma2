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
	if (isActive() == false) return;
	if (mesh == nullptr) return;
	mesh->draw(shader, ProjectionView, gameObject()->getGlobalTransform());
}

void StaticMeshComponent::DisableEvent()
{
	
}

void StaticMeshComponent::EnableEvent()
{
	
}
