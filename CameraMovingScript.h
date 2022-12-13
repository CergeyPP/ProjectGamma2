#pragma once
#include "ActorComponent.h"
#include "Application.h"
#include "ParticleSystem.h"

#include "InputMap.h"
#include <iostream>

#include <glm/gtx/string_cast.hpp>

class CameraMovingScript : public ActorComponent
{
public:

	CameraMovingScript(GameObject* parent) : ActorComponent(parent)	{  }

	~CameraMovingScript() override
	{
		Loader().getAsset<InputMap>("InputMap.txt")->actionMap["Jump"]->event -= MY_METHOD_HANDLER(CameraMovingScript::jump);
		Loader().getAsset<InputMap>("InputMap.txt")->actionMap["Move"]->event -= MY_METHOD_HANDLER(CameraMovingScript::move);
		Loader().getAsset<InputMap>("InputMap.txt")->actionMap["Look"]->event -= MY_METHOD_HANDLER(CameraMovingScript::look);
	}

	ParticleSystem* particles;

private:

	bool particleIsActive;
	float m_speed = 0;

	float m_pitch = 0;
	float m_yaw = 0;

	void beginPlay() override {
		m_speed = 5;
		Loader().getAsset<InputMap>("InputMap.txt")->actionMap["Jump"]->event += MY_METHOD_HANDLER(CameraMovingScript::jump);
		Loader().getAsset<InputMap>("InputMap.txt")->actionMap["Move"]->event += MY_METHOD_HANDLER(CameraMovingScript::move);
		Loader().getAsset<InputMap>("InputMap.txt")->actionMap["Look"]->event += MY_METHOD_HANDLER(CameraMovingScript::look);
		if (particles != nullptr)
			particleIsActive = particles->isActive();
	}

	void update() override {
		float delta = Application::get().deltaTime();
		//std::cout << "DeltaTime = " << delta << std::endl;
		//std::cout << "MovementVector: " << glm::to_string(moveVec) << std::endl;

		Transform transform = gameObject()->getLocalTransform();
		transform.position += delta * moveVec.x * transform.forward() * m_speed;
		transform.position += delta * moveVec.y * transform.right() * m_speed;

		gameObject()->setLocalTransform(transform);
	}

	glm::vec2 moveVec = glm::vec2(0);
	void move(const InputValue& value) {
		moveVec = value.axis;
	}

	void look(const InputValue& value) {
		//std::cout << "Look vector " << glm::to_string(value.axis) << std::endl;
		auto trans = gameObject()->getLocalTransform();
		
		m_pitch += value.axis.y / 1000;
		m_yaw -= value.axis.x / 1000;

		m_pitch = glm::clamp(m_pitch, -3.14f / 2, 3.14f / 2);
		trans.rotation = glm::vec3(m_pitch, m_yaw, 0);

		gameObject()->setLocalTransform(trans);
	}

	void jump(const InputValue& value) {
		std::cout << "Jump action!" << std::endl;
		if (particles != nullptr) {
			particleIsActive = !particleIsActive;
			particles->setActive(particleIsActive);
		}
	}
};