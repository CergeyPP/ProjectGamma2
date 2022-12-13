#pragma once
#include <vector>

#include "Transform.h"
#include "Component.h"

#include "ComponentLocator.h"

class GameObject
{
public:

	GameObject(Transform transform = Transform(), GameObject* parent = nullptr);
	~GameObject();

	template<class ...TParams>
	void foreachChildren(void (method)(GameObject*, TParams...), TParams... params) {
		for (auto& elem : m_children) {
			method(elem, params...);
		}
	}

	void addChild(GameObject* child);
	void removeChild(GameObject* child);

	Transform getGlobalTransform();
	Transform getLocalTransform();

	void setLocalTransform(Transform& transform);
	void setGlobalTransform(Transform& transform);

	template<class TComp>
	TComp* findComponent() {
		auto it = std::find_if(m_components.cbegin(), m_components.cend(), [](const Component* comp) {
			const TComp* comp = dynamic_cast<const TComp*>(comp);
			if (comp)
				return true;
			else return false;
			});

		if (it == m_components.end())
			return nullptr;
		return (*it);
	}

	template<class TComp, class ...TParams>
	TComp* createComponent(TParams... constructParams) {

		auto componentService = getComponentLocator().getLocator<TComp>();
		if (componentService == nullptr) {
			TComp* component = new TComp(this, constructParams...);
			m_components.push_back(component);
			return component;
		}
		else {
			return componentService->create(this, constructParams...);
		}

	}

	void deleteComponent(Component* component) {
		auto it = std::find_if(m_components.cbegin(), m_components.cend(), [&component](const Component* comp) {
			return (comp == component);
			});

		if (it != m_components.end()) {
			Component* deleting = *it;
			m_components.erase(it);
			delete deleting;
		}
	}

private:

	ComponentServiceLocator& getComponentLocator();

	Transform m_transform;
	GameObject* m_parent;
	std::vector<GameObject*> m_children;

	std::vector<Component*> m_components;
};