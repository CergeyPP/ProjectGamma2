#include "Framebuffers.h"
#include <iostream>

Framebuffer::Framebuffer(GLenum read, GLenum draw)
{
    glGenFramebuffers(1, &m_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
    glReadBuffer(read);
    glDrawBuffer(draw);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{

}

void Framebuffer::clear()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind()
{
    if (m_bindTextures.empty()) return;
    glViewport(0, 0, (int)m_bindTextures[0]->size().x, (int)m_bindTextures[0]->size().y);
    glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
}

void Framebuffer::unbind()
{
    if (m_bindTextures.empty()) return;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
}

void Framebuffer::attachTexture(Texture* texture, GLenum attachmentType)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
    int i;
    for (i = 0; i < m_bindTextures.size(); i++) {
        if (m_attachmentType[i] == attachmentType) {
            m_bindTextures.erase(m_bindTextures.begin() + i);
            m_attachmentType.erase(m_attachmentType.begin() + i);
            break;
        }
    }
    m_bindTextures.push_back(texture);
    m_attachmentType.push_back(attachmentType);

    glFramebufferTexture(GL_FRAMEBUFFER, attachmentType, texture->GL(), 0);
    glDrawBuffers(m_attachmentType.size(), &(m_attachmentType[0]));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::attachRenderbuffer(Renderbuffer* renderbuffer)
{
    m_attachedRenderBuffer = renderbuffer;

    glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer->GL());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Texture* Framebuffer::getTextureByAttachment(GLenum attachment) const
{
    int i;
    for (i = 0; i < m_attachmentType.size(); i++) {
        if (m_attachmentType[i] == attachment) break;
    }
    if (i == m_attachmentType.size()) return nullptr;

    return m_bindTextures[i];
}

Renderbuffer::Renderbuffer(int width, int height)
{
    glGenRenderbuffers(1, &m_buffer);
    //std::cout << glCheckError() << std::endl;
    glBindRenderbuffer(GL_RENDERBUFFER, m_buffer);
    //std::cout << glCheckError() << std::endl;
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    //std::cout << glCheckError() << std::endl;
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

Renderbuffer::~Renderbuffer()
{
    glDeleteRenderbuffers(1, &m_buffer);
}

GLuint Renderbuffer::GL()
{
    return m_buffer;
}
