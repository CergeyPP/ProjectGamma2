#include "ActorComponent.h"


ActorComponent::ActorComponent(GameObject* parent) : Component(parent)
{
	MainScene().beginPlayEvent += MY_METHOD_HANDLER(ActorComponent::onbeginPlay);
	MainScene().tickEvent += MY_METHOD_HANDLER(ActorComponent::update);
	MainScene().preRenderEvent += MY_METHOD_HANDLER(ActorComponent::preRender);
}

ActorComponent::~ActorComponent()
{
	DisableEvent();
}

void ActorComponent::DisableEvent()
{
	MainScene().beginPlayEvent -= MY_METHOD_HANDLER(ActorComponent::onbeginPlay);
	MainScene().tickEvent -= MY_METHOD_HANDLER(ActorComponent::update);
	MainScene().preRenderEvent -= MY_METHOD_HANDLER(ActorComponent::preRender);
	IPhysicComponent::DisableEvent();
}

void ActorComponent::EnableEvent()
{
	MainScene().beginPlayEvent += MY_METHOD_HANDLER(ActorComponent::onbeginPlay);
	MainScene().tickEvent += MY_METHOD_HANDLER(ActorComponent::update);
	MainScene().preRenderEvent += MY_METHOD_HANDLER(ActorComponent::preRender);
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
