#include "ActorComponent.h"


ActorComponent::ActorComponent(GameObject* parent) : Component(parent)
{
	Application::get().scene().beginPlayEvent += MY_METHOD_HANDLER(ActorComponent::onbeginPlay);
	Application::get().scene().tickEvent += MY_METHOD_HANDLER(ActorComponent::update);
	Application::get().scene().preRenderEvent += MY_METHOD_HANDLER(ActorComponent::preRender);
}

ActorComponent::~ActorComponent()
{
	DisableEvent();
}

void ActorComponent::DisableEvent()
{
	Application::get().scene().beginPlayEvent -= MY_METHOD_HANDLER(ActorComponent::onbeginPlay);
	Application::get().scene().tickEvent -= MY_METHOD_HANDLER(ActorComponent::update);
	Application::get().scene().preRenderEvent -= MY_METHOD_HANDLER(ActorComponent::preRender);
	IPhysicComponent::DisableEvent();
}

void ActorComponent::EnableEvent()
{
	Application::get().scene().beginPlayEvent += MY_METHOD_HANDLER(ActorComponent::onbeginPlay);
	Application::get().scene().tickEvent += MY_METHOD_HANDLER(ActorComponent::update);
	Application::get().scene().preRenderEvent += MY_METHOD_HANDLER(ActorComponent::preRender);
	IPhysicComponent::EnableEvent();
}

void ActorComponent::onPreparePhysics()
{
	prePhysics();
}

void ActorComponent::onFetchPhysics()
{
	postPhysics();
}
