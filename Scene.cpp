#include "Scene.h"
#include "Application.h"

#include "StaticMeshComponent.h"

#include <glm/gtx/string_cast.hpp>

Mesh* createCanvas() {
	std::vector<Vertex> vertices = {
		Vertex(glm::vec3(-1, 1, 0), glm::vec3(0), glm::vec2(0, 1)),
		Vertex(glm::vec3(1, 1, 0), glm::vec3(0), glm::vec2(1, 1)),
		Vertex(glm::vec3(-1, -1, 0), glm::vec3(0), glm::vec2(0, 0)),
		Vertex(glm::vec3(1, -1, 0), glm::vec3(0), glm::vec2(1, 0)),
	};

	std::vector<GLuint> indices = {
		0, 1, 2,
		1, 2, 3,
	};
	Mesh* mesh = new Mesh();
	mesh->init(vertices, indices);
	return mesh;
}

void Scene::init(const std::string& scenePath)
{
	m_simple = new Shader;
	m_simple->load("Assets/ShaderSimple.txt");

	GameObject* obj = instantiate();
	auto mesh = obj->createComponent<StaticMeshComponent>();
	mesh->mesh = createBox(glm::vec3(1));

	m_postProcessingShader = new Shader;
	m_postProcessingShader->load("Assets/PostProcessShader.txt");

	m_canvas = createCanvas();

	colorframeBuffer.attachTexture(new Texture(GL_TEXTURE_2D, GL_RGBA, Application::get().getWindowSize()), 1);
}

void Scene::update()
{
}

void Scene::draw()
{
	colorframeBuffer.clear();
	//depthFramebuffer.clear();

	glm::mat4 projection = glm::perspective(90.f / 2, Application::get().getWindowSize().x / Application::get().getWindowSize().y, 0.1f, 100.f);
	glm::mat4 view = glm::lookAt(glm::vec3(4, 3, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glm::mat4 pv = projection * view;
	colorframeBuffer.bind();
	drawCallEvent(pv, m_simple);
	colorframeBuffer.unbind();

	colorframeBuffer.bind();
	glm::mat4 CanvasPV = glm::mat4(1);
	Transform canvasT = Transform();
	glBindTexture(colorframeBuffer.Image()->target(), colorframeBuffer.Image()->GL());
	m_canvas->draw(m_postProcessingShader);
	colorframeBuffer.unbind();

	glBindTexture(colorframeBuffer.Image()->target(), colorframeBuffer.Image()->GL());
	m_canvas->draw(m_postProcessingShader);
}

GameObject* Scene::instantiate(GameObject* parent)
{
	GameObject* object = new GameObject(Transform(), parent);
	m_objects.push_back(object);
	return object;
}
