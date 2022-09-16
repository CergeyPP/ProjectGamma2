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

void StaticMeshComponent::drawCall(glm::mat4& ProjectionView, Shader* shader)
{
	if (mesh == nullptr) return;
	mesh->draw(shader, ProjectionView, gameObject()->getGlobalTransform());
}

void StaticMeshComponent::DisableEvent()
{
	MainScene().drawCallEvent -= MY_METHOD_HANDLER(StaticMeshComponent::drawCall);
}

void StaticMeshComponent::EnableEvent()
{
	MainScene().drawCallEvent += MY_METHOD_HANDLER(StaticMeshComponent::drawCall);
}
