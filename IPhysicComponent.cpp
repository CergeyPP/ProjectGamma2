#include "IPhysicComponent.h"
#include "Application.h"

void IPhysicComponent::DisableEvent()
{
	Application::get().scene().prePhysicsEvent -= MY_METHOD_HANDLER(IPhysicComponent::onPreparePhysics);
	Application::get().scene().postPhysicsEvent -= MY_METHOD_HANDLER(IPhysicComponent::onFetchPhysics);
}

void IPhysicComponent::EnableEvent()
{
	Application::get().scene().prePhysicsEvent += MY_METHOD_HANDLER(IPhysicComponent::onPreparePhysics);
	Application::get().scene().postPhysicsEvent += MY_METHOD_HANDLER(IPhysicComponent::onFetchPhysics);
}
