#include "Texture.h"
#include <SOIL/SOIL.h>

#include "GLCheckError.h"

#include <iostream>

Texture::Texture(GLenum target, GLenum internationalformat, GLenum format, GLenum type, glm::vec2 size, bool multisampled)
{
    m_target = target;
    m_size = size;
    m_format = format;
    m_internationalformat = internationalformat;
    m_type = type;
    m_layers = 0;
    glGenTextures(1, &m_texture);
    
    glBindTexture(m_target, m_texture);
    if (m_target == GL_TEXTURE_CUBE_MAP) {
        for (int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_internationalformat, (int)size.x, (int)size.y, 0, m_format, m_type, nullptr);
        }
        m_layers = 5;
    }
    else if (m_target == GL_TEXTURE_2D_MULTISAMPLE)
        glTexStorage2DMultisample(m_target, 4, m_internationalformat, (int)size.x, (int)size.y, GL_TRUE);
    else
        glTexImage2D(m_target, 0, m_internationalformat, (int)size.x, (int)size.y, 0, m_format, m_type, nullptr);

    glCheckError();

    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
    glTexImage2D(m_target, 0, m_internationalformat, (int)size.x, (int)size.y, 0, m_format, m_type, nullptr);
    glBindTexture(m_target, 0);
}

void Texture::load(const std::string& filePath)
{
    std::cout << "Try to load " << filePath << std::endl;
    int width, height;
    unsigned char* image = SOIL_load_image(filePath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
    if (sizeof(image) == 0) {
        printf("File didnt open");
        return;
    }

    generateTextureFromData(width, height, image);

    SOIL_free_image_data(image);
}

void Texture::generateTextureFromData(int width, int height, const unsigned char* data)
{
    m_type = GL_UNSIGNED_BYTE;
    m_size = glm::vec2(width, height);
    m_format = GL_RGBA;
    m_internationalformat = GL_RGBA;
    m_target = GL_TEXTURE_2D;
    m_layers = 0;

    if (m_texture)
        glDeleteTextures(1, &m_texture);

    glGenTextures(1, &m_texture);
    glBindTexture(m_target, m_texture);

    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexImage2D(m_target, 0, m_internationalformat, width, height, 0, m_format, m_type, data);
    glGenerateMipmap(m_target);
    glBindTexture(m_target, 0);
}

void Texture::setBorderColor(glm::vec4 color)
{
    glBindTexture(m_target, m_texture);
    glTexParameterfv(m_target, GL_TEXTURE_BORDER_COLOR, &(color[0]));
    glBindTexture(m_target, 0);
}

void Texture::setWrapMode(GLenum mode)
{
    glBindTexture(m_target, m_texture);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, mode);
    glBindTexture(m_target, 0);
}

size_t Texture::getLayerCount()
{
    return m_layers;
}

void Texture::genMipmaps()
{
    glBindTexture(m_target, m_texture);
    glGenerateMipmap(m_target);
    glBindTexture(m_target, 0);
}
