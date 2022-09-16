#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Asset.h"

class Texture : public IAsset
{
public:

	Texture(GLenum target = GL_TEXTURE_2D, GLenum intenationalformat = GL_RGBA, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE, glm::vec2 size = glm::vec2(1));
	~Texture();

	GLuint GL();
	GLenum target();

	glm::vec2 size();
	void resize(glm::vec2 size);

	void load(const std::string& filePath) override;
	void generateTextureFromData(int width, int height, unsigned char* data);

private:

	glm::vec2 m_size;
	GLenum m_target;
	GLenum m_type;
	GLuint m_texture;
	GLenum m_format;
	GLenum m_internationalformat;
};

