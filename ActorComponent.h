#pragma once
#include "Component.h"
#include "IPhysicComponent.h"

#include "Application.h"

class ActorComponent :
    public Component, IPhysicComponent
{
protected:

    ActorComponent(GameObject* parent);
    ActorComponent(const ActorComponent& other);

    virtual void onEnable() {}
    virtual void onDisable() {}

    virtual void beginPlay() {}
    virtual void update() {}
    virtual void prePhysics() {}
    virtual void postPhysics() {}
    virtual void preRender() {}

public:

    ~ActorComponent() override;

private:

    void onbeginPlay() {
        beginPlay();
        Application::get().scene().beginPlayEvent -= MY_METHOD_HANDLER(ActorComponent::onbeginPlay);
    }
    void onTick();
    void onPreRender();

    void DisableEvent() override final;
    void EnableEvent() override final;

    void onPreparePhysics() override final;
    void onFetchPhysics() override final;
};

