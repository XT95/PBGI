#ifndef DEF_CORE_FBO_H
#define DEF_CORE_FBO_H

#include <GL/glew.h>

#include "Engine/texture.h"

//-----------------------------------------------------------------------------
// Class Frame Buffer Object
//-----------------------------------------------------------------------------
class FBO
{
public:
       FBO(const unsigned &nbAttachement = 1, bool attachDepthTexture = false);
       ~FBO();

       void enable(void);
       void disable(void);


       void setSize(const int &width, const int &height );
       void setFormat(GLint format);

       GLenum getFormat(){return m_format;}

       void bind(const int &channel, const unsigned &texture = 0);
       void bindDepth(const int channel = 0);
       Texture *getTexture(const unsigned &texture);
private:
       void updateTexture();

       GLuint m_FrameBuffer;
       GLuint m_DepthRenderBuffer;
       Texture *m_ColorTextures;
       GLuint m_DepthTextureID;
       GLint  m_format;
       unsigned m_nbAttachement;
       int m_width, m_height;
};


#endif
