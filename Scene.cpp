#include "Scene.h"
#include "Application.h"

#include "StaticMeshComponent.h"
#include "Model.h"
#include "LightComponent.h"
#include "CameraMovingScript.h"
#include "InputMap.h"

#include <glm/gtx/string_cast.hpp>

#include "CreateCanvasMesh.h"

Scene::Scene()
{
	m_mainCamera = nullptr;
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

	GameObject* cameraObject = spawn(Transform(glm::vec3(0, 4, 4)));
	auto moveScript = cameraObject->createComponent<CameraMovingScript>();
	auto camera = cameraObject->createComponent<CameraComponent>();
	camera->FOV = 90.f;
	//camera->setRenderTarget(&colorFramebuffer);
	setMainCamera(camera);


	GameObject* lightObject = spawn(Transform(glm::vec3(0,4,0), glm::vec3(glm::radians(45.f), glm::radians(180.f), 0)));
	auto light = lightObject->createComponent<LightComponent>();
	light->type = LightType::DIRECTIONAL;
	light->diffuse = glm::vec4(0, 1, 0, 1);
	light->specular = glm::vec4(0, 1, 0, 1);

	for (int i = -2; i <= 2; i += 4) {
			GameObject* lightObject = spawn(Transform(glm::vec3(i, 4, 2), glm::vec3(glm::radians(45.f), glm::radians(180.f), 0)));
			auto light = lightObject->createComponent<LightComponent>();
			light->type = LightType::POINT;
			light->diffuse = glm::vec4(1,1,1,1);
			light->specular = glm::vec4(1,1,1,1);
			light->setDistance(40);
	}



	GameObject* floorObject = spawn(Transform(glm::vec3(0, -0.01, 0), glm::vec3(0), glm::vec3(40, 0.01, 40)));
	auto floor = floorObject->createComponent<StaticMeshComponent>();
	floor->mesh = Loader().getAsset<Model>("Cube/simpleCube.fbx");

	GameObject* obj = spawn();
	auto mesh = obj->createComponent<StaticMeshComponent>();
	mesh->mesh = Application::get().Loader().getAsset<Model>("egirl/egirl.obj");
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
		camera->render();
	}
	m_mainCamera->render();

	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(m_mainCamera->getRenderTexture()->target(), m_mainCamera->getRenderTexture()->GL());
	extend::getCanvas().draw(m_screenShader);
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
	else {
		if (m_mainCamera == camera) {
			m_mainCamera = nullptr;
		}
	}
}

void Scene::setMainCamera(CameraComponent* camera)
{
	deleteCamera(camera);
	m_mainCamera = camera;
}
