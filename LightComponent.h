#pragma once
#include "Component.h"
#include "Framebuffers.h"
#include "Shader.h"

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

    glm::vec4 diffuse;
    glm::vec4 specular;

    bool isShadowCast = 0;

    virtual LightType getType() = 0;
    
private:

    void DisableEvent() override;
    void EnableEvent() override;
};

class DirectLight
    : public LightComponent 
{
public:

    DirectLight(GameObject* parent) : LightComponent(parent) {

    }
    ~DirectLight() override {

    }

    LightType getType() override {
        return LightType::DIRECTIONAL;
    }
};

class PointLight : public LightComponent {
public:

    PointLight(GameObject* parent) : LightComponent(parent) {
        radius = 0;
    }
    ~PointLight() override {

    }

    LightType getType() override {
        return LightType::POINT;
    }

    float radius;
};

class SpotLight : public LightComponent {
public:

    SpotLight(GameObject* parent) : LightComponent(parent) {
        range = 0;
        innerCutoff = 0;
        outerCutoff = 0;
    }
    ~SpotLight() override {

    }

    LightType getType() override {
        return LightType::SPOT;
    }

    float range;
    float innerCutoff;
    float outerCutoff;
};