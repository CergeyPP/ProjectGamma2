#include "RenderPipeline.h"
#include "Application.h"

RenderPipeline::RenderPipeline()
{
	
}

RenderPipeline::~RenderPipeline()
{

}

void RenderPipeline::addDrawable(StaticMeshComponent* mesh)
{
	if (mesh == nullptr) return;
	auto it = std::find(m_drawables.cbegin(), m_drawables.cend(), mesh);
	if (it == m_drawables.end()) {
		m_drawables.push_back(mesh);
	}
}

void RenderPipeline::deleteDrawable(StaticMeshComponent* mesh)
{
	if (mesh == nullptr) return;
	auto it = std::find(m_drawables.cbegin(), m_drawables.cend(), mesh);
	if (it != m_drawables.end()) {
		m_drawables.erase(it);
	}
}

void RenderPipeline::addLight(LightComponent* light)
{
	if (light == nullptr) return;
	auto it = std::find(m_lights.cbegin(), m_lights.cend(), light);
	if (it == m_lights.end()) {
		m_lights.push_back(light);
	}
}

void RenderPipeline::deleteLight(LightComponent* light)
{
	if (light == nullptr) return;
	auto it = std::find(m_lights.cbegin(), m_lights.cend(), light);
	if (it != m_lights.end()) {
		m_lights.erase(it);
	}
}
