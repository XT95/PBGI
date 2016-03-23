#include "Engine/scenemanager.h"
#include "Engine/fast2dquad.h"

#include "demo.h"

#include <glm/glm.hpp>

SceneManager::SceneManager(const int &width, const int &height)
    : m_cam(width, height,glm::vec3(5.,2.,0.), -3.14159265*.5+.3,-.3)
{
    //OGL Init
    Fast2DQuad::instance().init();
    glClearColor(0., .0, .0, .0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);


    loadShader();
    setResolution(width,height);

    //HDR
    m_fbo.setFormat(GL_RGBA32F);
}

SceneManager::~SceneManager()
{
    m_shad.destroy();
    m_postFX.destroy();
    m_sky.destroy();

    for(unsigned i = 0; i < m_obj.size(); ++i)
        delete m_obj.at(i);
}

void SceneManager::loadShader()
{
    //Load shaders
    m_postFX.loadVertexFragment("Data/Shaders/postFX.vsh", "Data/Shaders/postFX.fsh");
    m_shad.loadVertexFragment("Data/Shaders/basic.vsh", "Data/Shaders/basic.fsh");
    m_sky.loadVertexFragment("Data/Shaders/sky.vsh", "Data/Shaders/sky.fsh");

    //Sends uniforms
    m_shad.bind();
        m_shad.sendUniform("viewProjectionMatrix", m_cam.getViewProjectionMatrix());
        m_shad.sendUniform("albedo",0);
        m_shad.sendUniform("lightDepth",1);
        m_shad.sendUniform("radiosity",2);

    m_postFX.bind();
        m_postFX.sendUniform("tex",0);
        m_postFX.sendUniform("resolution", glm::vec2(Demo::resolution.x,Demo::resolution.y));
    ShaderProgram::unbind();
}

void SceneManager::setResolution( const int width, const int height)
{
    if(height != 0)
    {
        glViewport(0,0, width,height);
        m_fbo.setSize(width,height);
        m_cam.computeProjection(width, height);
        m_shad.bind();
            m_shad.sendUniform("viewProjectionMatrix", m_cam.getViewProjectionMatrix());
        ShaderProgram::unbind();
        m_postFX.bind();
            m_postFX.sendUniform("resolution", glm::vec2(width,height));
        ShaderProgram::unbind();
    }
}


Object *SceneManager::addObject(const std::string &path , const std::string &filename)
{
    m_obj.push_back( new Object() );
    m_obj.back()->load(filename, path);

    return m_obj.back();
}

void SceneManager::draw()
{
    //Computm_fboLighte light depth
    m_light.enableDepthBuffer();
        glClear(GL_DEPTH_BUFFER_BIT);

        m_light.bindDepthShader();
            for(unsigned i = 0; i < m_obj.size(); i++)
            {
                m_obj.at(i)->drawMaterials(*m_light.getShader());
            }
        ShaderProgram::unbind();
    m_light.disableDepthBuffer();

    //Draw scene
    m_fbo.enable();
        glClear(GL_DEPTH_BUFFER_BIT);

        //Draw sky
        m_sky.bind();
        m_sky.sendUniform("mat", glm::inverse(m_cam.getViewMatrix()));
        m_sky.sendUniform("resolution", glm::vec2(Demo::resolution.x, Demo::resolution.y));
        m_sky.sendUniform("lightPos", m_light.getPosition() );
        glDisable(GL_DEPTH_TEST);
        Fast2DQuad::instance().render();
        glEnable(GL_DEPTH_TEST);

        //Draw objects
        m_light.bindDepthBuffer(1);
        m_shad.bind();
        m_shad.sendUniform("VPMatrix", m_cam.getViewProjectionMatrix());
        m_shad.sendUniform("lightVPMatrix", m_light.getViewProjectionMatrix());
        m_shad.sendUniform("lightPos", m_light.getPosition());
            for(unsigned i = 0; i < m_obj.size(); i++)
            {
                m_obj.at(i)->drawMaterials(m_shad);
            }
        ShaderProgram::unbind();

    m_fbo.disable();

    glViewport(0, 0, Demo::resolution.x, Demo::resolution.y);

    glDisable(GL_DEPTH_TEST);
    m_postFX.bind();
        m_fbo.bind(0);
        Fast2DQuad::instance().render();
    ShaderProgram::unbind();
    glEnable(GL_DEPTH_TEST);

}
