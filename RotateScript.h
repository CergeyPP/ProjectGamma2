#pragma once

#include "ActorComponent.h"
#include "Application.h"

class RotateScript : public ActorComponent {
public:

	RotateScript(GameObject* parent) : ActorComponent(parent) { }

	float rotateSpeed;

private:

	float yrotate;

	void beginPlay() override {
		yrotate = glm::eulerAngles(gameObject()->getLocalTransform().rotation).y;
	}

	void update() override {
		float delta = Application::get().deltaTime();

		yrotate += rotateSpeed * delta;

		glm::vec3 rotate = glm::vec3(0);
		rotate.y = yrotate;
		Transform transform = gameObject()->getLocalTransform();
		transform.rotation = glm::quat(rotate);

		gameObject()->setLocalTransform(transform);
	}

};