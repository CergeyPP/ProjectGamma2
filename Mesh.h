#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <utility>

#include "Transform.h"
#include "Material.h"

class IDrawable
{
public:
	virtual void draw(Shader* shader = nullptr, const glm::mat4& ProjectionView = glm::mat4(1), const Transform& transform = Transform()) = 0;
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoords;

	Vertex(glm::vec3 position, glm::vec3 normals, glm::vec2 TexCoords) {
		pos = position;
		normal = normals;
		texCoords = TexCoords;
	}

};

class Mesh : public IDrawable
{
public:

	Mesh(Material* material = nullptr);
	~Mesh();

	void draw(Shader* shader = nullptr, const glm::mat4& ProjectionView = glm::mat4(1), const Transform& transform = Transform()) override;

	void init(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);

	void setMaterial(Material* material) {
		if (m_material)
			delete m_material;

		m_material = material;
	}

private:

	Material* m_material;

	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	unsigned int m_indicesCount;

	GLuint VAO;
	GLuint EBO;
	GLuint VBO;
};

Mesh* createBox(glm::vec3 size = glm::vec3(1));