#pragma once
#include "Component.h"

enum class LightType : int32_t {
    DIRECTIONAL,
    POINT,
    SPOT,
};

class LightComponent :
    public Component
{
public:

    LightComponent(GameObject* object);
    ~LightComponent() override;

    LightType type;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float linear;
    float quadratic;

private:

    void DisableEvent() override;
    void EnableEvent() override;
};

