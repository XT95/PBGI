#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "demo.h"
#include "Engine/fast2dquad.h"

using namespace glm;


glm::ivec2 Demo::resolution = ivec2(1280,720);

Demo::Demo(const int width, const int height)
    : m_radiosity(glm::ivec2(128))
{
    resolution = glm::ivec2(width, height);

    init();
}

Demo::~Demo()
{

}

void Demo::init()
{
    //Debug shaders
    m_shaderDebug.loadVertexFragment("Data/Shaders/Utils/dbg.vsh", "Data/Shaders/Utils/dbg.fsh");
    m_shaderDebug.bind();
    m_shaderDebug.sendUniform("tex", 0);
    ShaderProgram::unbind();

    //Scene init
    m_scene = new SceneManager(resolution.x, resolution.y);
    m_cam = m_scene->getCamera();
    m_light = m_scene->getLight();

    Object *obj = m_scene->addObject("Data/Scenes//", "Data/Scenes/scene.obj");
    obj->setTexture("Data/Textures/albedo.jpg");
    m_radiosity.addObject( obj );



   /* m_cube = m_scene->addObject("Data/Scenes//", "Data/Scenes/cube.obj");
    m_cube->setTexture("Data/Scenes/cube.jpg");
    m_radiosity.addObject( m_cube );*/

    //Radiosity init
    m_radiosity.generateUVs();

    //Radiosity generate
    m_scene->draw(); //To compute light depth
    m_radiosity.generateBuffers(m_light);
    m_radiosity.computeIndirect();
}

void Demo::setResolution(const int width, const int height)
{
    Demo::resolution.x = width;
    Demo::resolution.y = height;

    if(m_scene)
        m_scene->setResolution(width, height);
}

void Demo::setMouseMovement(int x, int y)
{
    m_cam->horizontalMovement(x);
    m_cam->verticalMovement(y);
}

void Demo::moveCameraForward()
{
    m_cam->moveForward();
}

void Demo::moveCameraBackward()
{
    m_cam->moveBackward();
}

void Demo::moveCameraRight()
{
    m_cam->moveRight();
}

void Demo::moveCameraLeft()
{
    m_cam->moveLeft();
}

void Demo::modifyFov(int val)
{
    m_cam->setFov(m_cam->getFov()+val);
}

void Demo::reloadShader()
{
    m_radiosity.loadShader();
    m_scene->loadShader();

    //Recompute radiosity
    m_scene->draw();
    m_radiosity.generateBuffers(m_light);
    m_radiosity.computeIndirect();
}
void Demo::placeLightToCamera()
{
    //m_light->setProjectionMatrix(m_cam->getProjectionMatrix());
    m_light->setViewMatrix(m_cam->getViewMatrix());

    //Update system
    m_scene->draw();
    m_radiosity.generateBuffers(m_light);
    m_radiosity.computeIndirect();
}

void Demo::render()
{
    float t = m_clock.getTime()*.1;
    ///m_cube->setTransform( glm::rotate(glm::translate( glm::mat4(1.), glm::vec3(2.,0.,-1.)), t*1.5f, glm::vec3(0.5,1.,0.75)));
    m_light->setViewMatrix( glm::lookAt(glm::vec3(cos(t), 1.f, sin(t))*5.f, glm::vec3(0.), glm::vec3(0.,1.,0.)));

    m_radiosity.generateBuffers(m_light);
    m_radiosity.computeIndirect();
    m_radiosity.getIndirect()->bind(2);
    m_scene->draw();
}

void Demo::showDebug( float size )
{
    float ratio = float(resolution.x)/resolution.y;
    glDisable(GL_DEPTH_TEST);

    m_shaderDebug.bind();
    m_shaderDebug.sendUniform("scale", glm::vec2(size, size*ratio));
/*
        m_radiosity.getDataBuffer(0)->bind(0);
        m_shaderDebug.sendUniform("pos", glm::vec2(.0f));
        Fast2DQuad::instance().render();

        m_radiosity.getDataBuffer(1)->bind(0);
        m_shaderDebug.sendUniform("pos", glm::vec2(1.f*size,0.f));
        Fast2DQuad::instance().render();

        m_radiosity.getDataBuffer(2)->bind(0);
        m_shaderDebug.sendUniform("pos", glm::vec2(0.f, size*ratio));
        Fast2DQuad::instance().render();

        m_radiosity.getResBuffer()->bind(0);
        m_shaderDebug.sendUniform("pos", glm::vec2(1.f*size, size*ratio));
        Fast2DQuad::instance().render();

        m_light->getDepthBuffer()->bindDepth(0);
        m_shaderDebug.sendUniform("pos", glm::vec2(.0f,2.f*size*ratio));
        Fast2DQuad::instance().render();
*/
    m_radiosity.getDataBuffer(0)->bind(0);
    m_shaderDebug.sendUniform("pos", glm::vec2(.0f));
    Fast2DQuad::instance().render();

    m_radiosity.getDataBuffer(1)->bind(0);
    m_shaderDebug.sendUniform("pos", glm::vec2(1.f*size,0.f));
    Fast2DQuad::instance().render();

    m_radiosity.getDataBuffer(2)->bind(0);
    m_shaderDebug.sendUniform("pos", glm::vec2(0.f, size*ratio));
    Fast2DQuad::instance().render();

    m_radiosity.getDataBuffer(3)->bind(0);
    m_shaderDebug.sendUniform("pos", glm::vec2(1.f*size, size*ratio));
    Fast2DQuad::instance().render();

    ShaderProgram::unbind();

    glEnable(GL_DEPTH_TEST);
}
