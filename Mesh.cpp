#include "Mesh.h"

Mesh::Mesh(Material* material)
{
	m_material = material;
	m_indicesCount = 0;
	EBO = 0;
	VBO = 0;
	VAO = 0;
}

Mesh::~Mesh()
{
	if (m_material)
		delete m_material;
}

void Mesh::draw(Shader* shader, const glm::mat4& ProjectionView, const Transform& transform)
{
	if (shader == nullptr) {
		if (m_material == nullptr) return;
		m_material->mat4Param["ProjectionView"] = ProjectionView;
		m_material->mat4Param["Model"] = transform.matrix();
		m_material->use();
	}
	else {
		glUseProgram(shader->getProgram());
		shader->setUniform("ProjectionView", (glm::mat4&)ProjectionView);
		glm::mat4 model = transform.matrix();
		shader->setUniform("Model", model);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::init(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
	m_vertices.swap(vertices);
	m_indices.swap(indices);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &(m_vertices[0]), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &(m_indices[0]), GL_STATIC_DRAW);

    //vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    //vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_indicesCount = m_indices.size();
}

Mesh* createBox(glm::vec3 size) {
	std::vector<Vertex> vertices = {
		Vertex(glm::vec3(size.x, size.y, size.z),glm::vec3(1), glm::vec2(0)),
		Vertex(glm::vec3(-size.x, size.y, size.z),glm::vec3(1), glm::vec2(0)),
		Vertex(glm::vec3(size.x, -size.y, size.z),glm::vec3(1), glm::vec2(0)),
		Vertex(glm::vec3(-size.x, -size.y, size.z),glm::vec3(1), glm::vec2(0)),

		Vertex(glm::vec3(size.x, size.y, -size.z),glm::vec3(1), glm::vec2(0)),
		Vertex(glm::vec3(-size.x, size.y, -size.z),glm::vec3(1), glm::vec2(0)),
		Vertex(glm::vec3(size.x, -size.y, -size.z),glm::vec3(1), glm::vec2(0)),
		Vertex(glm::vec3(-size.x, -size.y, -size.z),glm::vec3(1), glm::vec2(0)),
	};
	std::vector<GLuint> indices = {
		0, 1, 2,
		1, 2, 3,

		0, 4, 1,
		4, 1, 5,

		0, 2, 4,
		2, 4, 6,

		1, 5, 3,
		5, 3, 7,

		2, 7, 3,
		2, 7, 6,

		4, 5, 6,
		5, 6, 7,
	};
	Mesh* mesh = new Mesh;
	mesh->init(vertices, indices);

	vertices.clear();
	indices.clear();

	return mesh;
}
