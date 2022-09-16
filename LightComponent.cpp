#include "LightComponent.h"
#include "Application.h"

LightComponent::LightComponent(GameObject* object) : Component(object)
{
	EnableEvent();
}

LightComponent::~LightComponent()
{
	DisableEvent();
}

void LightComponent::DisableEvent()
{
	
}

void LightComponent::EnableEvent()
{

}
