#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"

class Framebuffer
{
public:

	Framebuffer();
	virtual ~Framebuffer();

	virtual void clear() = 0;
	virtual void bind() = 0;
	virtual void unbind() = 0;
	Texture* Image();
	virtual void attachTexture(Texture* texture, bool isUnique) = 0;

protected:

	Texture* m_texture = nullptr;
	bool m_isTextureUnique = 0;
	GLuint m_buffer = -1;
};

class ColorFramebuffer : public Framebuffer
{
public:

	ColorFramebuffer();
	~ColorFramebuffer() override;

	void clear() override;
	void bind() override;
	void unbind() override;

	void attachTexture(Texture* texture, bool isUnique) override;

private:

	GLuint m_renderBuffer;
};

class DepthFramebuffer : public Framebuffer
{
public:

	DepthFramebuffer();
	~DepthFramebuffer() override;

	void clear() override;
	void bind() override;
	void unbind() override;

	void attachTexture(Texture* texture, bool isUnique) override;
};