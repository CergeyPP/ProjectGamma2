#pragma once

class GameObject;

class IComponent
{
protected:
	virtual void DisableEvent() = 0;
	virtual void EnableEvent() = 0;
};

class Component : public IComponent
{
protected:
	Component(GameObject* parent);
public:

	virtual ~Component();

	GameObject* gameObject();

	void setActive(bool active);
	bool isActive();

private:

	GameObject* m_gameObject;
	bool m_isActive;
};

#include "GameObject.h"