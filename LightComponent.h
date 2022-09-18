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

    LightType type;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float linear;
    float quadratic;
    float cutoff;

    void setDistance(float distance)
    {
        linear = 4.5 / distance;
        quadratic = 75 / distance / distance;
    }

    float getDistance(float distance)
    {
        return 4.5 / linear;
    }

private:

    void lightProcess(Framebuffer& sourceBuffer, Framebuffer& colorFramebuffer, glm::vec3 viewPos);

    void DisableEvent() override;
    void EnableEvent() override;

    Shader* m_depthPassShader;
    Shader* m_lightPassShader;
    Shader* m_directDepthPassShader;
    Shader* m_directLightPassShader;
};

