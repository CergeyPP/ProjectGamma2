#include "CreateCanvasMesh.h"

namespace extend {
	Mesh& getCanvas() {
		static std::vector<Vertex> vertices = {
			Vertex(glm::vec3(-1, 1, 0), glm::vec3(0), glm::vec2(0, 1)),
			Vertex(glm::vec3(1, 1, 0), glm::vec3(0), glm::vec2(1, 1)),
			Vertex(glm::vec3(-1, -1, 0), glm::vec3(0), glm::vec2(0, 0)),
			Vertex(glm::vec3(1, -1, 0), glm::vec3(0), glm::vec2(1, 0)),
		};

		static std::vector<GLuint> indices = {
			1, 0, 2,
			1, 2, 3,
		};
		static bool initF = 0;
		static Mesh mesh;
		if (!initF) {
			mesh.init(vertices, indices);
			initF = 1;
			vertices.clear();
			indices.clear();
		}
		return mesh;
	}
}