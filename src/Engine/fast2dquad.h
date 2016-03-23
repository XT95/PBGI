#ifndef FAST2DQUAD_H
#define FAST2DQUAD_H

#include <GL/glew.h>
#include "Utils/shaderprogram.h"

class Fast2DQuad
{
public:
    static Fast2DQuad& instance();
    void init();
    void render();
    void free();


    void enableBasicShading();
    void disableBasicShading();
private:
    Fast2DQuad();
    ~Fast2DQuad();

    ShaderProgram m_shader;
    GLuint m_vbo,m_vao;
    static Fast2DQuad m_instance;
};

#endif // FAST2DQUAD_H
