#include "Scene.h"
#include "Application.h"

#include "StaticMeshComponent.h"
#include "Model.h"
#include "LightComponent.h"
#include "CameraMovingScript.h"
#include "InputMap.h"

#include <glm/gtx/string_cast.hpp>

Scene::Scene()
{
	m_mainCamera = nullptr;
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
	GameObject* obj = spawn();
	auto mesh = obj->createComponent<StaticMeshComponent>();
	mesh->mesh = Application::get().Loader().getAsset<Model>("egirl/egirl.obj");
		//createBox(glm::vec3(1));

	GameObject* cameraObject = spawn(Transform(glm::vec3(4, 3, 2)));
	auto moveScript = cameraObject->createComponent<CameraMovingScript>();
	auto camera = cameraObject->createComponent<CameraComponent>();
	camera->FOV = 90.f;
	//camera->setRenderTarget(&colorFramebuffer);
	setMainCamera(camera);

	GameObject* lightObject = spawn(Transform(glm::vec3(0, 4, 0)));
	auto light = lightObject->createComponent<LightComponent>();
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
	m_mainCamera->render();


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
