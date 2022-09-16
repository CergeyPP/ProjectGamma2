
#define _CRT_SECURE_NO_WARNINGS

#include "Application.h"
#include <windows.h>
#include <fstream>
#include <iostream>

//#include "Mesh.h"
#include "InputMap.h"


Application::Application()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_window = nullptr;
	m_windowSize = glm::vec2(0);
}

Application::~Application()
{
	m_window = nullptr;
	m_windowSize = glm::vec2(0);
}

glm::vec2 Application::getWindowSize()
{
	return m_windowSize;
}

void Application::init()
{
	static const char* filename = "Assets/config.cfg";

	std::fstream config(filename);
	if (!config.is_open())
		exit(1);

	std::string title;
	config >> title;

	int w, h;
	config >> w >> h;

	m_windowSize.x = w;
	m_windowSize.y = h;
	m_window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(m_window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << glewGetErrorString(GLEW_VERSION) << std::endl;
	}

	glDepthFunc(GL_LESS);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CW);

	glfwSetWindowUserPointer(m_window, this);
	glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
	glfwSetCursorPosCallback(m_window, MouseMovedCallback);
	glfwSetKeyCallback(m_window, KeyCallback);
	glfwSetScrollCallback(m_window, ScrollCallback);

	m_scene = new Scene;
	m_scene->init("");
}

void Application::start()
{
	float currentTime = 0;
	glfwSetTime(0);

	while (!glfwWindowShouldClose(m_window)) {

		float time = glfwGetTime();
		float deltaTime = time - currentTime;
		currentTime = time;

		m_deltaTime = deltaTime;
		m_time = currentTime;

		glfwPollEvents();

		m_scene->update();

		m_scene->draw();

		glfwSwapBuffers(m_window);

	}

	clearResources();
}

AssetLoader& Application::Loader()
{
	return m_loader;
}

Scene& Application::scene()
{
	return *m_scene;
}

void Application::clearResources()
{


	glfwTerminate();
}

void Application::KeyCallback(GLFWwindow* window, int button, int scancode, int action, int mods)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);

	app->m_loader.forEach<InputMap>([&button, &action, &mods](InputMap* map) {
		map->broadcastKeyEvent(button, action, mods);
		}
	);
}

void Application::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);

	app->m_loader.forEach<InputMap>([&button, &action, &mods](InputMap* map) {
		map->broadcastMouseButtonEvent(button, action, mods);
		}
	);
}

void Application::MouseMovedCallback(GLFWwindow* window, double xpos, double ypos)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);

	glm::vec2 offset = glm::vec2(xpos, ypos) - app->m_mousePos;
	app->m_mousePos = glm::vec2(xpos, ypos);

	app->m_loader.forEach<InputMap>([&offset](InputMap* map) {
		map->broadcastMouseMovedEvent(offset.x, offset.y);
		}
	);
}

void Application::ScrollCallback(GLFWwindow* window, double xpos, double ypos)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);

	app->m_loader.forEach<InputMap>([&xpos, &ypos](InputMap* map) {
		map->broadcastMouseMovedEvent(xpos, ypos);
		}
	);
}

void Application::CloseCallback(GLFWwindow* window)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);

	app->m_loader.forEach<InputMap>([](InputMap* map) {
		map->broadcastCloseEvent();
		}
	);
}
