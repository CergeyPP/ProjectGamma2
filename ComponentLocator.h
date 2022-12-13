#pragma once
#include <map>
#include <functional>
#include "Component.h"
#include "StaticMeshComponent.h"

class IService {

};

template<class TComp>
class ComponentService : public IService{
public:

	
	template< class ...TParams>
	TComp* create(TParams... params) {
		m_components.emplace_back(TComp(params...));
		return &m_components[m_components.size() - 1];
	}

	bool destroy(TComp* component) {
		auto it = std::find(m_components.cbegin(), m_components.cend(), &component);
		if (it != m_components.end()) {
			m_components.erase(it);
			return true;
		}
		return false;
	}

	TComp* getByIndex(size_t index) {
		if (index >= m_components.size()) return nullptr;
		return &m_components[index];
	}

	void foreach(std::function<void(TComp&)> func) {
		for (auto& elem : m_components) {
			func(elem);
		}
	}


private:

	std::vector<TComp> m_components;
};

class ComponentServiceLocator {
public:

	template<class TComp>
	void registerLocator(ComponentService<TComp>* locator) {
		m_locators[typeid(TComp).name()] = locator;
	}

	template<class TComp>
	void deleteLocator(ComponentService<TComp>* locator) {
		m_locators[typeid(TComp).name()] = nullptr;
	}

	template<class TComp>
	ComponentService<TComp>* getLocator() {
		if (m_locators.find(typeid(TComp).name()) != m_locators.end())
			return (ComponentService<TComp>*)m_locators[typeid(TComp).name()];
		else return nullptr;
	}



private:

	std::map<const char*, IService*> m_locators;
};