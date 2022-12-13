#include "ActorComponent.h"


ActorComponent::ActorComponent(GameObject* parent) : Component(parent)
{
	MainScene().beginPlayEvent += MY_METHOD_HANDLER(ActorComponent::onbeginPlay);
	MainScene().tickEvent += MY_METHOD_HANDLER(ActorComponent::onTick);
	MainScene().preRenderEvent += MY_METHOD_HANDLER(ActorComponent::onPreRender);
}

ActorComponent::ActorComponent(const ActorComponent& other)
	: Component(other)
{
	MainScene().beginPlayEvent += MY_METHOD_HANDLER(ActorComponent::onbeginPlay);
	MainScene().tickEvent += MY_METHOD_HANDLER(ActorComponent::onTick);
	MainScene().preRenderEvent += MY_METHOD_HANDLER(ActorComponent::onPreRender);
}

ActorComponent::~ActorComponent()
{
	DisableEvent();
}

void ActorComponent::onTick()
{
	update();
}

void ActorComponent::onPreRender()
{
	preRender();
}

void ActorComponent::DisableEvent()
{
	MainScene().beginPlayEvent -= MY_METHOD_HANDLER(ActorComponent::onbeginPlay);
	MainScene().tickEvent -= MY_METHOD_HANDLER(ActorComponent::onTick);
	MainScene().preRenderEvent -= MY_METHOD_HANDLER(ActorComponent::onPreRender);
	IPhysicComponent::DisableEvent();
}

void ActorComponent::EnableEvent()
{
	MainScene().beginPlayEvent += MY_METHOD_HANDLER(ActorComponent::onbeginPlay);
	MainScene().tickEvent += MY_METHOD_HANDLER(ActorComponent::onTick);
	MainScene().preRenderEvent += MY_METHOD_HANDLER(ActorComponent::onPreRender);
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
