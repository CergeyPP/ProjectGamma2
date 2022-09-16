#pragma once
#include "ActorComponent.h"
#include "Application.h"

#include "InputMap.h"
#include <iostream>

#include <glm/gtx/string_cast.hpp>

class CameraMovingScript : public ActorComponent
{
public:

	CameraMovingScript(GameObject* parent) : ActorComponent(parent)	{  }

	~CameraMovingScript() override
	{
		Application::get().Loader().getAsset<InputMap>("InputMap.txt")->actionMap["Jump"]->event -= MY_METHOD_HANDLER(CameraMovingScript::jump);
	}

private:

	float m_speed = 0;

	void beginPlay() override {
		m_speed = 0;
		Application::get().Loader().getAsset<InputMap>("InputMap.txt")->actionMap["Jump"]->event += MY_METHOD_HANDLER(CameraMovingScript::jump);
		Application::get().Loader().getAsset<InputMap>("InputMap.txt")->actionMap["Move"]->event += MY_METHOD_HANDLER(CameraMovingScript::move);

	}

	void update() override {
		float delta = Application::get().deltaTime();
		//std::cout << "DeltaTime = " << delta << std::endl;
		//std::cout << "MovementVector: " << glm::to_string(moveVec) << std::endl;

		Transform transform = gameObject()->getLocalTransform();
		transform.position += delta * moveVec.x * transform.forward();
		transform.position += delta * moveVec.y * transform.right();

		gameObject()->setLocalTransform(transform);
	}

	glm::vec2 moveVec = glm::vec2(0);
	void move(const InputValue& value) {
		if (moveVec != value.axis)
			std::cout << "MovementVector: " << glm::to_string(value.axis) << std::endl;
		moveVec = value.axis;
	}

	void jump(const InputValue& value) {
		std::cout << "Jump action!" << std::endl;
	}
};