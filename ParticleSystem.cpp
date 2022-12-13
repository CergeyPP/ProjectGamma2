#include "ParticleSystem.h"

const static Vertex vertices[] = {
			Vertex(glm::vec3(-1, 1, 0), glm::vec3(0), glm::vec2(0, 1)),
			Vertex(glm::vec3(1, 1, 0), glm::vec3(0), glm::vec2(1, 1)),
			Vertex(glm::vec3(-1, -1, 0), glm::vec3(0), glm::vec2(0, 0)),
			Vertex(glm::vec3(1, -1, 0), glm::vec3(0), glm::vec2(1, 0)),
};

const GLuint indices[] = {
	1, 0, 2,
	1, 2, 3,
};

float random() {
	return float(rand() % 256) / 256.f;
}

ParticleSystem::ParticleSystem(GameObject* object)
	: ActorComponent(object)
{
	//setActive(0);
	std::cout << "CREATE PARTICLE\n";
}

ParticleSystem::ParticleSystem(ParticleSystem&& other) noexcept 
	: ActorComponent(other)
{
	m_isInit = other.m_isInit;
	m_acceleration = other.m_acceleration;
	m_maxLifeTime = other.m_maxLifeTime;
	m_maxParticles = other.m_maxParticles;
	m_ParticleCount = other.m_ParticleCount;
	m_particles = other.m_particles;
	m_particlesSize = other.m_particlesSize;
	m_randAcceleration = other.m_randAcceleration;
	m_randVelocity = other.m_randVelocity;
	m_shader = other.m_shader;
	m_spawnDelta = other.m_spawnDelta;
	m_spawnInterval = other.m_spawnInterval;
	m_texture = other.m_texture;
	m_velocity = other.m_velocity;
	VAO = other.VAO;
	VBO = other.VBO;
	EBO = other.EBO;
	particleVBO = other.particleVBO;

	other.m_isInit = 0;
	other.m_particles = nullptr;
	other.m_maxParticles = 0;
	other.m_ParticleCount = 0;
	other.VAO = -1;
	other.VBO = -1;
	other.EBO = -1;
	other.particleVBO = -1;
}

ParticleSystem::~ParticleSystem()
{
	if (m_isInit) {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &particleVBO);
		delete[] m_particles;
	}
}

void ParticleSystem::init(ParticleSystemConfig config)
{
	m_acceleration = config.acceleration;
	m_maxLifeTime = config.maxLifeTime;
	m_randAcceleration = config.randomAcceleration;
	m_velocity = config.velocity;
	m_randVelocity = config.randomVelocity;
	m_spawnInterval = config.spawnInterval;

	m_texture = config.texture;
	m_particlesSize = config.particleSize;

	m_isInit = m_maxLifeTime > 0 && m_spawnInterval > 0 && m_texture != nullptr && m_particlesSize.x > 0 && m_particlesSize.y > 0;

	if (!m_isInit) return;
	m_maxParticles = (m_maxLifeTime / m_spawnInterval) + 1;
	m_particles = new Particle[m_maxParticles];

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &particleVBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(Particle), m_particles, GL_STREAM_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, lifetime));
	glVertexAttribDivisor(4, 1);

	glBindVertexArray(0);

	std::cout << "Size of particle system: " << m_maxParticles << std::endl;

	//setActive(1);
}

void ParticleSystem::draw(const glm::mat4& ProjectionView)
{
	if (!m_isInit) return;
	if (!isActive()) return;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_maxParticles * sizeof(Particle), m_particles);

	CameraComponent* camera = Application::get().getRenderCamera();
	m_shader->setUniform("ProjectionView", ProjectionView);
	m_shader->setUniform("View", camera->getViewMatrix());
	m_shader->setUniform("size", m_particlesSize);
	m_shader->setTexture("Texture", *m_texture);
	m_shader->setUniform("maxLifetime", m_maxLifeTime);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_ParticleCount);
	glBindVertexArray(0);
}

void ParticleSystem::update()
{
	if (!m_isInit) return;
	if (!isActive()) return;

	float delta = Application::get().deltaTime();

	for (int i = 0; i < m_ParticleCount; i++) {
		m_particles[i].lifetime -= delta;
		if (m_particles[i].lifetime <= 0) {
			m_ParticleCount--;
			m_particles[i] = m_particles[m_ParticleCount];
			i--;
			continue;
		}

		m_particles[i].velocity += m_particles[i].acceleration * delta;
		m_particles[i].position += m_particles[i].velocity * delta;
	}

	m_spawnDelta -= delta;
	if (m_spawnDelta <= 0) {
		for (m_spawnDelta; m_spawnDelta < 0; m_spawnDelta += m_spawnInterval) {
			if (m_ParticleCount >= m_maxParticles) continue;
			Particle particle;
			particle.acceleration = m_acceleration + 
				2.f * glm::vec3(m_randAcceleration.x * random(), m_randAcceleration.y * random(), m_randAcceleration.z * random()) 
				- m_randAcceleration;
			particle.velocity = m_velocity +
				2.f * glm::vec3(m_randVelocity.x * random(), m_randVelocity.y * random(), m_randVelocity.z * random())
				- m_randVelocity + particle.acceleration * -m_spawnDelta;
			particle.position = gameObject()->getGlobalTransform().position + particle.velocity * -m_spawnDelta;
			particle.lifetime = m_maxLifeTime;

			m_particles[m_ParticleCount] = particle;
			m_ParticleCount++;
		}
	}
}

void ParticleSystem::onEnable()
{
	if (!m_isInit) return;
}

void ParticleSystem::onDisable()
{
	if (!m_isInit) return;
}
