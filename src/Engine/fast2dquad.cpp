#include "fast2dquad.h"


Fast2DQuad Fast2DQuad::m_instance=Fast2DQuad();

Fast2DQuad::Fast2DQuad()
{

}

Fast2DQuad::~Fast2DQuad()
{
    m_shader.destroy();
}

Fast2DQuad& Fast2DQuad::instance()
{
    return m_instance;
}

void Fast2DQuad::init()
{
    float vertex[4][3];
    vertex[0][0] =-1.0f; vertex[1][0] = 1.0f; vertex[2][0] =-1.0f; vertex[3][0] = 1.0f;
    vertex[0][1] =-1.0f; vertex[1][1] =-1.0f; vertex[2][1] = 1.0f; vertex[3][1] = 1.0f;
    for(int i=0; i<4; i++)
        vertex[i][2] = 0.f;

    //make the vertex buffer object
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 4*3*sizeof(float), vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //make the vertex array object
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindVertexArray(0);

    VertexShader vShader;
    FragmentShader fShader;

    m_shader.loadVertexFragment("Data/Shaders/Utils/basic.vsh", "Data/Shaders/Utils/basic.fsh");

    m_shader.bind();
    m_shader.sendUniform("tex", 0);
    ShaderProgram::unbind();
}
void Fast2DQuad::render()
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
void Fast2DQuad::free()
{
    glDeleteBuffers(1,&m_vbo);
    glDeleteVertexArrays(1,&m_vao);

}


void Fast2DQuad::enableBasicShading()
{
    m_shader.bind();
}


void Fast2DQuad::disableBasicShading()
{
    ShaderProgram::unbind();
}
