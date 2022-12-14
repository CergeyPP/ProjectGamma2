#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Asset.h"
#include "RenderPipeline.h"

#include "ComponentLocator.h"

class Scene;

class Application
{
public:

	static Application& get() { static Application app; return app; }

	glm::vec2 getWindowSize();

	void init();
	void start();

	AssetLoader& Loader();
	Scene& scene();

	glm::vec2 getMousePos() {
		return m_mousePos;
	}

	float getTime() {
		return m_time;
	}
	float deltaTime() {
		return m_deltaTime;
	}

	RenderPipeline& getPipeline() {
		return *m_pipeline;
	}

	ComponentServiceLocator& getServiceLocator() {
		return m_locator;
	}

	CameraComponent* getRenderCamera() {
		return m_pipeline->getRenderCamera();
	}

private:

	float m_time;
	float m_deltaTime;

	AssetLoader m_loader;
	Scene* m_scene = nullptr;

	void clearResources();

	glm::vec2 m_windowSize;

	GLFWwindow* m_window;

	glm::vec2 m_mousePos;

	Application();
	~Application();

	static void KeyCallback(GLFWwindow* window, int button, int scancode, int action, int mods);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MouseMovedCallback(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallback(GLFWwindow* window, double xpos, double ypos);
	static void CloseCallback(GLFWwindow* window);

	RenderPipeline* m_pipeline;

	ComponentServiceLocator m_locator;
};

AssetLoader& Loader();
Scene& MainScene();

#include "Scene.h"