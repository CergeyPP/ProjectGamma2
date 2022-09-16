#pragma once
#include "Shader.h"
#include "GameObject.h"
#include "TEvent.h"
#include "Mesh.h"
#include "CameraComponent.h"

#include <vector>

class Scene
{
public:

	Scene();
	~Scene();

	void init(const std::string& scenePath);

	extend::TEvent<glm::mat4&, Shader*> drawCallEvent;
	extend::TEvent<Framebuffer&, Framebuffer&> lightProcessEvent;

	//logicEvents
	extend::TEvent<> beginPlayEvent;
	extend::TEvent<> tickEvent;

	extend::TEvent<> prePhysicsEvent;
	extend::TEvent<> postPhysicsEvent;

	extend::TEvent<> preRenderEvent;
	//logicEventsEnd

	void update();
	void draw();

	GameObject* spawn(const Transform& transform = Transform(), GameObject* parent = nullptr);
	void kill(GameObject* object);

	void addCamera(CameraComponent* camera);
	void deleteCamera(CameraComponent* camera);

private:

	void setMainCamera(CameraComponent* camera);

	std::vector<CameraComponent*> m_cameras;
	CameraComponent* m_mainCamera;

	std::vector<GameObject*> m_instancedObjects;
	std::vector<GameObject*> m_objects;
	std::vector<GameObject*> m_deadObjects;
};

