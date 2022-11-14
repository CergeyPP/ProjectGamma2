#include "Scene.h"
#include "Application.h"

#include "StaticMeshComponent.h"
#include "Model.h"
#include "LightComponent.h"
#include "CameraMovingScript.h"
#include "RotateScript.h"
#include "InputMap.h"

#include <glm/gtx/string_cast.hpp>

#include "CreateCanvasMesh.h"

Scene::Scene()
{
	m_screenShader = Loader().getAsset<Shader>("Screen.shader");
}

Scene::~Scene()
{
	for (auto& elem : m_deadObjects) {
		delete elem;
	}
	m_deadObjects.clear();

	for (auto& elem : m_objects) {
		delete elem;
	}
	m_objects.clear();

	for (auto& elem : m_instancedObjects) {
		delete elem;
	}
	m_instancedObjects.clear();
}

void Scene::init(const std::string& scenePath)
{

	GameObject* cameraObject = spawn(Transform(glm::vec3(2, 4, 4), glm::vec3(glm::radians(45.f), glm::radians(210.f), 0)));
	auto moveScript = cameraObject->createComponent<CameraMovingScript>();
	auto camera = cameraObject->createComponent<CameraComponent>();
	camera->FOV = 90.f;
	//camera->setRenderTarget(&colorFramebuffer);

	GameObject* lightObject = spawn(Transform(glm::vec3(0,6,0), glm::vec3(glm::radians(45.f), glm::radians(210.f), 0)));
	auto light = lightObject->createComponent<DirectLight>();
	light->diffuse = glm::vec4(1, 1, 1, 0.7);
	light->specular = glm::vec4(1, 1, 1, 0.7);
	light->isShadowCast = 1;
	GameObject* lightMObject; /*= spawn(Transform(glm::vec3(0), glm::vec3(0), glm::vec3(0.1, 0.1, 1)), lightObject);
	auto lightM = lightMObject->createComponent<StaticMeshComponent>();
	lightM->mesh = Loader().getAsset<Model>("Cube/simpleCube.fbx");*/

	GameObject* floorObject = spawn(Transform(glm::vec3(0, -0.02, 0), glm::vec3(0), glm::vec3(40, 0.01, 40)));
	auto floor = floorObject->createComponent<StaticMeshComponent>();
	floor->mesh = Loader().getAsset<Model>("Cube/simpleCube.fbx");

	lightObject = spawn(Transform(glm::vec3(-2, 4, 1)));
	auto Plight = lightObject->createComponent<PointLight>();

	Plight->diffuse = glm::vec4(0.2, 1, 0.2, 8);
	Plight->specular = glm::vec4(0.2, 1, 0.2, 8);
	Plight->radius = 6;

	Plight->isShadowCast = 0;

	lightObject = spawn(Transform(glm::vec3(2, 4, 1)));
	Plight = lightObject->createComponent<PointLight>();

	Plight->diffuse = glm::vec4(1, 0.2, 0.2, 12);
	Plight->specular = glm::vec4(1, 0.2, 0.2, 12);
	Plight->radius = 6;

	Plight->isShadowCast = 0;

	for (int i = -2; i <= 2; i+=2) {
		for (int j = -2; j <= 2; j+=2) {
			GameObject* obj = spawn(Transform(glm::vec3(i, 0, j), glm::vec3(0), glm::vec3(1)));
			auto mesh = obj->createComponent<StaticMeshComponent>();
			mesh->mesh = Application::get().Loader().getAsset<Model>("egirl/egirl.obj");

			auto rotator = obj->createComponent<RotateScript>();
			rotator->rotateSpeed = 0.2 + (i - j) / 2;
		}
	}
	
	//createBox(glm::vec3(1));
}

void Scene::update()
{
	for (auto& elem : m_instancedObjects) {
		m_objects.push_back(elem);
	}
	m_instancedObjects.clear();

	beginPlayEvent();

	tickEvent();

	prePhysicsEvent();
	//
	// PHYSX COMMANDS
	//
	postPhysicsEvent();

	preRenderEvent();
}

void Scene::draw()
{
	for (auto& camera : m_cameras) {
		Application::get().getPipeline().drawFrom(camera);
	}

	Application::get().getPipeline().showOnScreen();

	//glClear(GL_COLOR_BUFFER_BIT);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(m_mainCamera->getRenderTexture()->target(), m_mainCamera->getRenderTexture()->GL());
	//extend::getCanvas().draw(m_screenShader);
}

GameObject* Scene::spawn(const Transform& transform, GameObject* parent)
{
	GameObject* object = new GameObject(transform, parent);
	m_instancedObjects.push_back(object);
	return object;
}

void Scene::kill(GameObject* object)
{
	auto it = std::find(m_objects.begin(), m_objects.end(), object);
	if (it != m_objects.end()) {
		m_deadObjects.push_back(*it);
		m_objects.erase(it);
	}
	else {
		it = std::find(m_instancedObjects.begin(), m_instancedObjects.end(), object);
		if (it != m_instancedObjects.end()) {
			m_deadObjects.push_back(*it);
			m_instancedObjects.erase(it);
		}
	}
}

void Scene::addCamera(CameraComponent* camera)
{
	auto it = std::find(m_cameras.cbegin(), m_cameras.cend(), camera);
	if (it == m_cameras.end()) {
		m_cameras.push_back(camera);
	}
}

void Scene::deleteCamera(CameraComponent* camera)
{
	auto it = std::find(m_cameras.cbegin(), m_cameras.cend(), camera);
	if (it != m_cameras.end()) {
		m_cameras.erase(it);
	}
}