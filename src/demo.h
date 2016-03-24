#ifndef DEMO_H
#define DEMO_H

#include <glm/glm.hpp>
#include "Engine/scenemanager.h"
#include "Engine/camera.h"
#include "Engine/light.h"
#include "GI/radiosity.h"
#include "Utils/clock.h"
#include "Utils/shaderprogram.h"

class Demo
{
public:
    Demo( const int width, const int height );
    ~Demo();

    void init();
    void setResolution( const int width, const int height );
    void render();
    void showDebug( float size = .1f);
    void setMouseMovement(int x, int y);
    void moveCameraForward();
    void moveCameraBackward();
    void moveCameraRight();
    void moveCameraLeft();
    void modifyFov(int val);

    void reloadShader();
    void placeLightToCamera();

    static glm::ivec2 resolution;
private:
    Clock m_clock;

    SceneManager *m_scene;
    Radiosity m_radiosity;

    Object *m_cube;

    Light *m_light;
    Camera *m_cam;

    ShaderProgram m_shaderDebug;
};

#endif // DEMO_H
