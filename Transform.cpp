#include "Transform.h"
#include <glm/gtx/matrix_decompose.hpp>

glm::mat4 Transform::matrix() const
{
    glm::mat4 mat;
    mat = glm::translate(glm::mat4(1), position);
    mat *= glm::mat4_cast(rotation);
    mat = glm::scale(mat, scale);

    return mat;
}

void Transform::fromMatrix(glm::mat4& matrix)
{
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(matrix, scale, rotation, position, skew, perspective);
}

glm::vec3 Transform::forward()
{
    return matrix() * glm::vec4(0, 0, 1, 0);
}

glm::vec3 Transform::back()
{
    return matrix() * glm::vec4(0, 0, -1, 0);
}

glm::vec3 Transform::right()
{
    return matrix() * glm::vec4(-1, 0, 0, 0);
}

glm::vec3 Transform::left()
{
    return matrix() * glm::vec4(1, 0, 0, 0);
}

glm::vec3 Transform::up()
{
    return matrix() * glm::vec4(0, 1, 0, 0);
}

glm::vec3 Transform::down()
{
    return matrix() * glm::vec4(0, -1, 0, 0);
}
