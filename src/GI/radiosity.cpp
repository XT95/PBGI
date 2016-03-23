#include "GI/radiosity.h"
#include "Engine/fast2dquad.h"
#include "Utils/clock.h"

Radiosity::Radiosity( glm::ivec2 size)
    : m_uvmap(size),
      m_current(false),
      m_bufferData(3),
      m_size(size),
      m_offset(0,0)
{
    m_bufferData.setSize(size.x, size.y);
    m_bufferData.setFormat(GL_RGBA32F);

    for(int i=0; i<2; i++)
    {
        m_computeTex[i].load(NULL, size.x, size.y, GL_RGBA32F, GL_RGBA, GL_UNSIGNED_BYTE);
        m_computeTex[i].setFiltering(GL_NEAREST);

        m_indirectTex[i].load(NULL, size.x, size.y, GL_RGBA32F, GL_RGBA, GL_UNSIGNED_BYTE);
        m_indirectTex[i].setFiltering(GL_NEAREST);
    }
    m_output.load(NULL, size.x, size.y, GL_RGBA32F, GL_RGBA, GL_UNSIGNED_BYTE);
    m_output.setFiltering(GL_LINEAR);

    loadShader();
    m_state = FREE;
    m_timeFactor = 1.f;
}

Radiosity::~Radiosity()
{
    for(int i=0; i<4; i++)
        m_shader[i].destroy();
}


void Radiosity::addObject( Object *obj )
{
    m_obj.push_back(obj);
}


void Radiosity::generateUVs()
{
    m_uvmap.computeLightmapPack(m_obj);
    for(unsigned i=0; i<m_obj.size(); i++)
        m_obj[i]->updateGPUBuffers();
}

void Radiosity::generateBuffers(Light *light)
{
    if(m_state == COMPUTE1 || m_state == COMPUTE2)
        return;
    m_state = COMPUTE1;

    glDisable(GL_DEPTH_TEST);
    m_bufferData.enable();

    //Multiple output in single pass
    GLenum attachements[] ={GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachements);

    glClear(GL_COLOR_BUFFER_BIT);
    light->bindDepthBuffer(1);
    m_shader[0].bind();
        m_shader[0].sendUniform("lightVPMatrix", light->getViewProjectionMatrix());
        m_shader[0].sendUniform("lightPos", light->getPosition());
        for(unsigned j=0; j<m_obj.size(); j++)
            m_obj[j]->drawMaterials(m_shader[0]);
    ShaderProgram::unbind();

    m_bufferData.disable();

    glDrawBuffers(1, attachements);
    glEnable(GL_DEPTH_TEST);

    m_offset = glm::ivec2(0);
}

void Radiosity::computeIndirect()
{
    m_shader[1].bind();
    m_shader[1].sendUniform("offset", m_offset);
    m_bufferData.bind(0);
    m_bufferData.bind(1, 1);

    //Compute bounce
    if(m_state == COMPUTE1)
    {
        m_bufferData.bind(2, 2);
        m_computeTex[0].bindAsImage(3, GL_READ_WRITE);
    }
    else if(m_state == COMPUTE2)
    {
        m_computeTex[0].bind(2);
        m_computeTex[1].bindAsImage(3, GL_READ_WRITE);
    }
    glDispatchCompute(m_size.x/16, m_size.y/16, 1);
    m_offset.x += 32;
    if(m_offset.x == m_size.x)
    {
        m_offset.x = 0;
        m_offset.y += 32;
        if(m_offset.y == m_size.y-32 && m_state == COMPUTE1) //Last pass ?
        {
            m_shader[1].sendUniform("lastPass",1);
        }
        else if(m_offset.y == m_size.y) //Finish this texture ?
        {
            m_offset.y = 0;
            m_shader[1].sendUniform("lastPass",0);
            if(m_state == COMPUTE1) //Finish only the first bounce ?
            {
                m_state = COMPUTE2;
            }
            else
            {
				m_state = FREE;
                //Copy buffers
                m_current = !m_current;
                m_shader[2].bind();
                m_computeTex[1].bind(0);
                m_indirectTex[m_current].bindAsImage(1, GL_READ_WRITE);
                glDispatchCompute(m_size.x/16, m_size.y/16, 1);

                m_timeFactor = m_clock.getElapsedTime()*1.2;
                std::cout << "Radiosity done in " << m_clock.getElapsedTime() << std::endl;
                m_clock.reset();
            }
        }
    }

    //Final output (interpolation between last & current buffer)
    m_shader[3].bind();
    m_shader[3].sendUniform("t", (float)m_clock.getElapsedTime()/m_timeFactor);
    m_indirectTex[!m_current].bind(0);
    m_indirectTex[m_current].bind(1);
    m_output.bindAsImage(2, GL_READ_WRITE);
    glDispatchCompute(m_size.x/16, m_size.y/16, 1);

    glFlush();
    glFinish();

    ShaderProgram::unbind();
}

void Radiosity::loadShader()
{
    m_shader[0].destroy();
    m_shader[1].destroy();
    m_shader[2].destroy();
    m_shader[3].destroy();


    m_shader[0].loadVertexFragment("Data/Shaders/GI/buffers.vsh", "Data/Shaders/GI/buffers.gsh", "Data/Shaders/GI/buffers.fsh");
    m_shader[1].loadCompute("Data/Shaders/GI/radiosity.csh");
    m_shader[2].loadCompute("Data/Shaders/GI/copy.csh");
    m_shader[3].loadCompute("Data/Shaders/GI/timelerp.csh");

    m_shader[0].bind();
    m_shader[0].sendUniform("texSize", m_size);
    m_shader[0].sendUniform("albedo",0);
    m_shader[0].sendUniform("lightDepth",1);

    m_shader[1].bind();
    m_shader[1].sendUniform("lastPass",0);
    m_shader[1].sendUniform("position", 0);
    m_shader[1].sendUniform("direction", 1);
    m_shader[1].sendUniform("albedo", 2);
    m_shader[1].sendUniform("destTex", 3);

    m_shader[2].bind();
    m_shader[2].sendUniform("input", 0);
    m_shader[2].sendUniform("destTex", 1);

    m_shader[3].bind();
    m_shader[3].sendUniform("last", 0);
    m_shader[3].sendUniform("current", 1);
    m_shader[3].sendUniform("destTex", 2);

    ShaderProgram::unbind();
}

Texture* Radiosity::getIndirect()
{
    return &m_output;
}

Texture* Radiosity::getDataBuffer(const unsigned &d)
{
    if(d <= 1)
        return &m_computeTex[d];
    else if (d <= 3)
        return &m_indirectTex[d-2];
    else
        return m_bufferData.getTexture(d-4);
}
