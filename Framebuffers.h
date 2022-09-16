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

	GLuint GL() {
		return m_buffer;
	}

	//sdfgsdfhg
	void attachTexture(Texture* texture, GLenum attachmentType);
	void attachRenderbuffer(Renderbuffer* renderbuffer);

private:

	GLuint m_buffer = -1;
	std::vector<Texture*> m_bindTextures;
	std::vector<GLuint> m_attachmentType;

	Renderbuffer* m_attachedRenderBuffer;
};

class Renderbuffer {
public:

	Renderbuffer(int width, int height);
	~Renderbuffer();

	GLuint GL();
private:

	GLuint m_buffer;
};
