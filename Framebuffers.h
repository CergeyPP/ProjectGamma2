#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Texture.h"

class Renderbuffer;

class Framebuffer
{
public:

	Framebuffer(GLenum read = GL_FRONT, GLenum draw = GL_FRONT);
	virtual ~Framebuffer();

	void clear();
	void bind();
	void unbind();

	void copy(Framebuffer& copyTarget, GLbitfield bitmask);

	GLuint GL() {
		return m_buffer;
	}

	void setClearColor(glm::vec4 color);

	//sdfgsdfhg
	virtual void attachTexture(Texture* texture, GLenum attachmentType);
	virtual void attachRenderbuffer(Renderbuffer* renderbuffer);

	Texture* getTextureByAttachment(GLenum attachment) const;

protected:

	GLuint m_buffer = -1;
	std::vector<Texture*> m_bindTextures;
	std::vector<GLuint> m_attachmentType;

	glm::vec4 m_clearColor = glm::vec4(0);

	Renderbuffer* m_attachedRenderBuffer;
};

class FramebufferMultiSample : public Framebuffer {
public:

	FramebufferMultiSample(GLenum read = GL_FRONT, GLenum draw = GL_FRONT) : Framebuffer(read, draw) { }
	~FramebufferMultiSample() override { }

	void attachTexture(Texture* texture, GLenum attachmentType) override;

};

class Renderbuffer {
public:

	Renderbuffer(int width, int height);
	virtual ~Renderbuffer();

	GLuint GL();

	glm::vec2 size();
protected:

	GLuint m_buffer;
	glm::vec2 m_size;
};

class RenderbufferMultisample : public Renderbuffer {
public:

	RenderbufferMultisample(int width, int height);
	~RenderbufferMultisample() override;


};
