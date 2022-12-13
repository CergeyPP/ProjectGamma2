#include "Billboard.h"
#include "Application.h"
#include "CreateCanvasMesh.h"

Billboard::Billboard()
{
	size = glm::vec2(0);
	m_renderTexture = nullptr;
	m_shader = Loader().getAsset<Shader>("Billboard.shader");
}

Billboard::~Billboard()
{
}

void Billboard::load(const std::string& filename)
{
	m_renderTexture = Loader().getAsset<Texture>(filename.substr(filename.find_first_of('/') + 1, filename.length()-1));
	size = m_renderTexture->size();
}

void Billboard::draw(Shader* shader, const glm::mat4& ProjectionView, const Transform& transform)
{
	if (size.x <= 0) return;
	if (size.y <= 0) return;

	CameraComponent* camera = Application::get().getRenderCamera();
	if (camera == nullptr) return;

	Transform trans = transform;
	trans.scale = glm::vec3(size, 1);

	//if (shader != nullptr) return;
	m_shader->setTexture("Texture", *m_renderTexture);
	m_shader->setUniform("centerPos", trans.position);
	m_shader->setUniform("size", size);
	m_shader->setUniform("viewM", camera->getViewMatrix());
	
	glUseProgram(m_shader->getProgram());

	extend::getCanvas().draw(m_shader, ProjectionView, Transform());
}

void Billboard::setTexture(Texture* texture)
{
}
