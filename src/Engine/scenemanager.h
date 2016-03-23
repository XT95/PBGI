#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H


#include "Engine/camera.h"
#include "Engine/object.h"
#include "Engine/camera.h"
#include "Engine/light.h"

#include "Utils/fbo.h"
#include "Utils/shaderprogram.h"

class SceneManager
{
public:
    SceneManager(const int &width, const int &height);
    ~SceneManager();

    void loadShader();
    void setResolution( const int width, const int height);
    Object *addObject(const std::string &path , const std::string &filename);

    Light* getLight() { return &m_light; }
    Camera* getCamera() { return &m_cam; }
    FBO* getFBO() { return &m_fbo; }

    void draw();
private:
    Camera m_cam;
    Camera m_camLight;
    std::vector<Object*> m_obj;
    ShaderProgram m_shad;
    ShaderProgram m_sky;
    ShaderProgram m_postFX;

    FBO m_fbo;

    Light m_light;
};

#endif // SCENEMANAGER_H
