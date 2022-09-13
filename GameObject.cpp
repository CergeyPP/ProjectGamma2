#include "GameObject.h"

GameObject::~GameObject()
{
	for (auto& elem : m_children) {
		delete elem;
	}
	m_children.clear();
}

GameObject::GameObject(Transform transform, GameObject* parent)
{
	m_transform = transform;
	m_parent = parent;
}

void GameObject::addChild(GameObject* child)
{
	child->m_parent = this;
	m_children.push_back(child);
}

void GameObject::removeChild(GameObject* child)
{
	auto it = std::find_if(m_children.cbegin(), m_children.cend(), [&child](const GameObject* elem) {
		return (child == elem);
		});

	if (it != m_children.end()) {
		GameObject* deleting = *it;
		m_children.erase(it);
		deleting->m_parent = nullptr;
	}
}

Transform GameObject::getGlobalTransform()
{
	if (m_parent == nullptr)
		return m_transform;
	else return m_parent->getGlobalTransform() * m_transform;
}

Transform GameObject::getLocalTransform()
{
	return m_transform;
}

void GameObject::setLocalTransform(Transform& transform)
{
	m_transform = transform;
}

void GameObject::setGlobalTransform(Transform& transform)
{
	if (m_parent == nullptr) {
		m_transform = transform;
	}
	else {
		Transform trans;
		glm::mat4 matrix = glm::inverse(m_parent->getGlobalTransform().matrix() * transform.matrix());
		trans.fromMatrix(matrix);
		m_transform = trans;
	}
}
