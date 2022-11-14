#pragma once
#include <map>
#include "Component.h"
#include "StaticMeshComponent.h"

class IService {

};

template<class TComp>
class ComponentService : public IService{
public:

	TComp* create(GameObject* parent) {
		m_components.push_back(TComp(parent));
		return &m_components[m_components.size() - 1];
	}

	void destroy(TComp* component) {
		
	}


private:

	std::vector<TComp> m_components;
};

class ComponentServiceLocator {
public:

	template<class TComp>
	void registerLocator(ComponentService<TComp>* locator) {
		m_locators[typeid(TComp).hash_code()] = locator;
	}

	template<class TComp>
	ComponentService<TComp>* getLocator() {
		if (m_locators.find(typeid(TComp).hash_code()) != m_locators.end())
			return (ComponentService<TComp>*)m_locators[typeid(TComp).hash_code()];
		else return nullptr;
	}

private:

	std::map<size_t, IService*> m_locators;
};