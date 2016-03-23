#include "Utils/fbo.h"
#include "demo.h"

#include <iostream>


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
FBO::FBO(const unsigned &nbAttachement, bool attachDepthTexture)
    : m_nbAttachement(nbAttachement)
{
    m_FrameBuffer=0;
    m_ColorTextures = new Texture[nbAttachement];
    m_width = 1280;
    m_height = 720;
    m_format = GL_RGBA;

    //Create texture.

    for(unsigned i = 0; i < m_nbAttachement; ++i)
    {
        m_ColorTextures[i].load(NULL, m_width, m_height, m_format, GL_RGBA, GL_UNSIGNED_BYTE);
    }

    glGenTextures(1, &m_DepthTextureID);
    glBindTexture(GL_TEXTURE_2D, m_DepthTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE,   GL_LUMINANCE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,   GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,   GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    //Make buffer
    glGenFramebuffers(1, &m_FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);


    for(unsigned i = 0; i < m_nbAttachement; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, m_ColorTextures[i].getID(), 0);
    }
    if(attachDepthTexture)
    {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthTextureID, 0);
    }
    else
    {
        glGenRenderbuffers(1, &m_DepthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, m_DepthRenderBuffer);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
FBO::~FBO()
{
    glDeleteTextures(1, &m_DepthTextureID);
    glDeleteFramebuffers(1, &m_FrameBuffer);
    glDeleteRenderbuffers(1, &m_DepthRenderBuffer);
    delete []m_ColorTextures;
}


//-----------------------------------------------------------------------------
// Enable
//-----------------------------------------------------------------------------
void FBO::enable(void)
{
    glViewport(0,0,m_width,m_height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
}

//-----------------------------------------------------------------------------
// Disable
//-----------------------------------------------------------------------------
void FBO::disable(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,Demo::resolution.x,Demo::resolution.y);
}

//-----------------------------------------------------------------------------
// Set
//-----------------------------------------------------------------------------
void FBO::setSize(const int &width, const int &height)
{
    m_width = width;
    m_height = height;

    updateTexture();
}

void FBO::setFormat(GLint format)
{
    m_format = format;
    updateTexture();
}

//-----------------------------------------------------------------------------
// updateTexture
//-----------------------------------------------------------------------------
void FBO::updateTexture()
{
    for(unsigned i = 0; i < m_nbAttachement; ++i)
    {
        m_ColorTextures[i].setFormat(m_format);
        m_ColorTextures[i].setSize(m_width, m_height);
        m_ColorTextures[i].updateTexture();
    }

    glBindTexture(GL_TEXTURE_2D, m_DepthTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

//-----------------------------------------------------------------------------
// bind
//-----------------------------------------------------------------------------
void FBO::bind(const int &channel, const unsigned &texture)
{
    if(texture < m_nbAttachement)
        m_ColorTextures[texture].bind(channel);
}
void FBO::bindDepth(const int channel)
{
    glActiveTexture(GL_TEXTURE0+channel);
    glBindTexture(GL_TEXTURE_2D, m_DepthTextureID);
}

Texture *FBO::getTexture(const unsigned &texture)
{
    if(texture < m_nbAttachement)
        return &m_ColorTextures[texture];

    return NULL;
}
