#pragma once
#include "Component.h"
class IPhysicComponent :
    public IComponent
{
protected:

    virtual void onPreparePhysics() = 0;
    virtual void onFetchPhysics() = 0;

    void DisableEvent() override;
    void EnableEvent() override;
};

