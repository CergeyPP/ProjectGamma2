#pragma once
#include "Component.h"
#include "Mesh.h"

class StaticMeshComponent
	: public Component
{
public:

	StaticMeshComponent(GameObject* parent);
	~StaticMeshComponent() override;

	IDrawable* mesh;
	bool isOpaque = 1;

	void drawCall(const glm::mat4& ProjectionView, Shader* shader = nullptr);

private:

	void DisableEvent() override;
	void EnableEvent() override;
};

