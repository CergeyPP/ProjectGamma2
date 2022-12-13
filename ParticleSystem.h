#pragma once
#include "ActorComponent.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include <vector>

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float lifetime;
};

struct ParticleSystemConfig {
    glm::vec3 velocity;
    glm::vec3 randomVelocity = glm::vec3(0);
    glm::vec3 acceleration;
    glm::vec3 randomAcceleration = glm::vec3(0);
    float maxLifeTime;
    float spawnInterval;
    Texture* texture;

    glm::vec2 particleSize = glm::vec2(0.1, 0.1);
};

class ParticleSystem :
    public ActorComponent
{
public:

    ParticleSystem(GameObject* object);
    ParticleSystem(ParticleSystem&& other) noexcept;
    ~ParticleSystem();

    void init(ParticleSystemConfig config);

    void draw(const glm::mat4& ProjectionView);

private:

    bool m_isInit = 0;

    float m_spawnDelta = 0;
    float m_maxLifeTime = 0;
    float m_spawnInterval = 0;
    glm::vec3 m_velocity = glm::vec3(0);
    glm::vec3 m_acceleration = glm::vec3(0);
    glm::vec3 m_randVelocity = glm::vec3(0);
    glm::vec3 m_randAcceleration = glm::vec3(0);

    Particle* m_particles = nullptr;
    size_t m_maxParticles = 0;

    size_t m_ParticleCount = 0;

    Texture* m_texture = nullptr;
    Shader* m_shader = Loader().getAsset<Shader>("particle.shader");
    glm::vec2 m_particlesSize = glm::vec2(0);
    GLuint VAO, VBO, EBO;

    GLuint particleVBO = -1;

    void update() override;

    void onEnable() override;
    void onDisable() override;
};

