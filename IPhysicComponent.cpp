#include "IPhysicComponent.h"
#include "Application.h"

void IPhysicComponent::DisableEvent()
{
	MainScene().prePhysicsEvent -= MY_METHOD_HANDLER(IPhysicComponent::onPreparePhysics);
	MainScene().postPhysicsEvent -= MY_METHOD_HANDLER(IPhysicComponent::onFetchPhysics);
}

void IPhysicComponent::EnableEvent()
{
	MainScene().prePhysicsEvent += MY_METHOD_HANDLER(IPhysicComponent::onPreparePhysics);
	MainScene().postPhysicsEvent += MY_METHOD_HANDLER(IPhysicComponent::onFetchPhysics);
}
