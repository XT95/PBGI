#ifndef LIGHT_H
#define LIGHT_H

#include "Engine/camera.h"
#include "Utils/fbo.h"
#include "Utils/shaderprogram.h"

class Light
{
public:
    Light();
    ~Light();


    void setResolution( const int width, const int height);
    void setProjectionMatrix( const glm::mat4 &mat );
    void setViewMatrix( const glm::mat4 &mat );

    glm::mat4 getProjectionMatrix() const;
    glm::mat4 getViewMatrix() const;
    glm::mat4 getViewProjectionMatrix() const;
    glm::vec3 getPosition() const { return m_pos; }

    FBO* getDepthBuffer();

    ShaderProgram* getShader() { return &m_shader; }
    void enableDepthBuffer();
    void disableDepthBuffer();

    void bindDepthBuffer(const int &channel);
    void bindDepthShader();

private:
    Camera m_cam;
    glm::vec3 m_pos;
    FBO m_depth;

    ShaderProgram m_shader;
};

#endif // LIGHT_H
