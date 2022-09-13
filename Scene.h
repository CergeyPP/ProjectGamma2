#pragma once
#include "Shader.h"
#include "GameObject.h"
#include "TEvent.h"
#include "Framebuffers.h"
#include "Mesh.h"

#include <vector>

class Scene
{
public:

	void init(const std::string& scenePath);

	extend::TEvent<glm::mat4&, Shader*> drawCallEvent;
	extend::TEvent<Framebuffer&, Framebuffer&> lightProcessEvent;

	void update();
	void draw();

	GameObject* instantiate(GameObject* parent = nullptr);

private:

	std::vector<GameObject*> m_objects;

	Shader* m_simple;
	Shader* m_postProcessingShader;

	Mesh* m_canvas;

	ColorFramebuffer colorframeBuffer;
	DepthFramebuffer depthFramebuffer;
};

