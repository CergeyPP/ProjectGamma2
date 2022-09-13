#include "Texture.h"
#include <SOIL/SOIL.h>

Texture::Texture(GLenum target, GLenum format, glm::vec2 size, GLenum type)
{
    m_target = target;
    m_size = size;
    m_format = format;
    m_type = type;

    glGenTextures(1, &m_texture);
    glBindTexture(m_target, m_texture);

    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexImage2D(m_target, 0, format, (int)size.x, (int)size.y, 0, format, m_type, nullptr);
    glGenerateMipmap(m_target);
    glBindTexture(m_target, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_texture);
}

GLuint Texture::GL()
{
    return m_texture;
}

GLenum Texture::target()
{
    return m_target;
}

glm::vec2 Texture::size()
{
    return m_size;
}

void Texture::resize(glm::vec2 size) {

    m_size = size;

    glBindTexture(m_target, m_texture);
    glTexImage2D(m_target, 0, m_format, (int)size.x, (int)size.y, 0, m_format, m_type, nullptr);
    glBindTexture(m_target, 0);
}

void Texture::load(const std::string& filePath)
{
    int width, height;
    unsigned char* image = SOIL_load_image(filePath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
    if (sizeof(image) == 0) {
        printf("File didnt open");
        return;
    }

    generateTextureFromData(width, height, image);

    SOIL_free_image_data(image);
}

void Texture::generateTextureFromData(int width, int height, unsigned char* data)
{
    if (m_texture)
        glDeleteTextures(1, &m_texture);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_type = GL_UNSIGNED_BYTE;
    m_size = glm::vec2(width, height);
    m_format = GL_RGBA;
    m_target = GL_TEXTURE_2D;
}
