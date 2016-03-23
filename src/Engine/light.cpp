#include "light.h"

Light::Light() :
      m_depth(0,true)
{
    m_depth.setSize(4096,4096);
    m_cam.setProjectionMatrix(glm::ortho(-10.f,10.f,-10.f,10.f,-5.f,20.f)); //View matrix defined in demo.cpp
    m_cam.setViewMatrix( glm::mat4(1.f) );

    m_shader.loadVertexFragment("Data/Shaders/depth.vsh","Data/Shaders/depth.fsh");
}

Light::~Light()
{

}


void Light::setResolution(const int width, const int height)
{
    m_depth.setSize(width,height);
}

void Light::setProjectionMatrix( const glm::mat4 &mat )
{
    m_cam.setProjectionMatrix( mat );
}

void Light::setViewMatrix( const glm::mat4 &mat )
{
    m_cam.setViewMatrix( mat );
    glm::mat4 inv = glm::inverse(m_cam.getViewMatrix());
    m_pos.x = inv[3][0];
    m_pos.y = inv[3][1];
    m_pos.z = inv[3][2];
}

glm::mat4 Light::getProjectionMatrix() const
{
    return m_cam.getProjectionMatrix();
}
glm::mat4 Light::getViewMatrix() const
{
    return m_cam.getViewMatrix();
}
glm::mat4 Light::getViewProjectionMatrix() const
{
    return m_cam.getViewProjectionMatrix();
}
FBO* Light::getDepthBuffer()
{
    return &m_depth;
}

void Light::bindDepthShader()
{
    m_shader.bind();
    m_shader.sendUniform("viewProjectionMatrix", m_cam.getViewProjectionMatrix());

}
void Light::bindDepthBuffer(const int &channel)
{
    m_depth.bindDepth(channel);
}

void Light::enableDepthBuffer()
{
    m_depth.enable();
}
void Light::disableDepthBuffer()
{
    m_depth.disable();
}
