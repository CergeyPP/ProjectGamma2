#include "Framebuffers.h"

Framebuffer::Framebuffer()
{
}

Framebuffer::~Framebuffer()
{
}

void Framebuffer::bind()
{
}

void Framebuffer::unbind()
{
}

Texture* Framebuffer::Image()
{
    return m_texture;
}

ColorFramebuffer::ColorFramebuffer()
{
    glGenFramebuffers(1, &m_buffer);
    m_renderBuffer = 0;
}

ColorFramebuffer::~ColorFramebuffer()
{
    if (!m_isTextureUnique)
        delete m_texture;
    glDeleteFramebuffers(1, &m_buffer);
    if (m_renderBuffer)
        glDeleteRenderbuffers(1, &m_renderBuffer);
}

void ColorFramebuffer::clear()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
    glClear(/*GL_COLOR_BUFFER_BIT | */GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ColorFramebuffer::bind()
{
    if (m_texture == nullptr)
        return;
    glViewport(0, 0, (int)m_texture->size().x, (int)m_texture->size().y);
    glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
}

void ColorFramebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
}

void ColorFramebuffer::attachTexture(Texture* texture, bool isUnique)
{
    if (m_isTextureUnique)
        delete m_texture;
    
    m_texture = texture;
    m_isTextureUnique = isUnique;

    glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->GL(), 0);

    if (m_renderBuffer)
        glDeleteRenderbuffers(1, &m_renderBuffer);
    glGenRenderbuffers(1, &m_renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (int)m_texture->size().x, (int)m_texture->size().y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DepthFramebuffer::DepthFramebuffer()
{
    glGenFramebuffers(1, &m_buffer);
}

DepthFramebuffer::~DepthFramebuffer()
{
    glDeleteFramebuffers(1, &m_buffer);
    if (m_isTextureUnique)
        delete m_texture;
}

void DepthFramebuffer::clear()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
    glClear(GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthFramebuffer::bind()
{
    if (m_texture == nullptr)
        return;
    glViewport(0, 0, (int)m_texture->size().x, (int)m_texture->size().y);
    glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
    glEnable(GL_DEPTH_TEST);
}

void DepthFramebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
}

void DepthFramebuffer::attachTexture(Texture* texture, bool isUnique)
{
    if (m_isTextureUnique)
        delete m_texture;

    m_texture = texture;
    m_isTextureUnique = isUnique;

    glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_texture->GL(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
