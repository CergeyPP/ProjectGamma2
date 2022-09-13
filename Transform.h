#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{
public:

	Transform(glm::vec3 pos = glm::vec3(0), glm::quat rot = glm::quat(1,0,0,0), glm::vec3 scal = glm::vec3(1)) {
		position = pos;
		rotation = rot;
		scale = scal;
	}

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	glm::mat4 matrix() const;
	void fromMatrix(glm::mat4& matrix);

	glm::vec3 forward();
	glm::vec3 back();
	glm::vec3 right();
	glm::vec3 left();
	glm::vec3 up();
	glm::vec3 down();

	Transform operator*(const Transform& other) {
		glm::mat4 result = matrix() * other.matrix();
		Transform trans;
		trans.fromMatrix(result);
		return trans;
	}
};

