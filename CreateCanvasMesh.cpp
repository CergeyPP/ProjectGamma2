#include "CreateCanvasMesh.h"

namespace extend {
	Mesh* createCanvas() {
		std::vector<Vertex> vertices = {
			Vertex(glm::vec3(-1, 1, 0), glm::vec3(0), glm::vec2(0, 1)),
			Vertex(glm::vec3(1, 1, 0), glm::vec3(0), glm::vec2(1, 1)),
			Vertex(glm::vec3(-1, -1, 0), glm::vec3(0), glm::vec2(0, 0)),
			Vertex(glm::vec3(1, -1, 0), glm::vec3(0), glm::vec2(1, 0)),
		};

		std::vector<GLuint> indices = {
			0, 1, 2,
			1, 2, 3,
		};
		Mesh* mesh = new Mesh();
		mesh->init(vertices, indices);

		vertices.clear();
		indices.clear();

		return mesh;
	}
}