#include "Component.h"

Component::Component(GameObject* parent)
{
    m_gameObject = parent;
    m_isActive = 1;
}

Component::~Component()
{
}

GameObject* Component::gameObject()
{
    return m_gameObject;
}

void Component::setActive(bool active)
{
    if (m_isActive == active) return;
    m_isActive = active;
    if (active)
        EnableEvent();
    else DisableEvent();
}

bool Component::isActive()
{
    return m_isActive;
}
