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

	std::vector<CameraComponent*> m_cameras;

	std::vector<GameObject*> m_instancedObjects;
	std::vector<GameObject*> m_objects;
	std::vector<GameObject*> m_deadObjects;

	Shader* m_screenShader;
};

