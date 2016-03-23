#ifndef RADIOSITY_H
#define RADIOSITY_H

#include <glm/glm.hpp>
#include "GI/uvmapper.h"
#include "Engine/object.h"
#include "Engine/camera.h"
#include "Engine/light.h"
#include "Utils/shaderprogram.h"
#include "Utils/fbo.h"
#include "Utils/clock.h"

class Radiosity
{
public:
    Radiosity( glm::ivec2 size);
    ~Radiosity();

    void addObject( Object *obj );

    void generateUVs();
    void generateBuffers( Light *light );

    void computeIndirect();
    void loadShader();

    Texture *getIndirect();
    Texture *getDataBuffer(const unsigned &d);
private:
    Clock m_clock;
    std::vector<Object*> m_obj;
    UVMapper m_uvmap;
    bool m_current;

    FBO m_bufferData;
    Texture m_computeTex[2];
    Texture m_indirectTex[2];
    Texture m_output;

    ShaderProgram m_shader[4];

    float m_timeFactor;
    glm::ivec2 m_size;
    glm::ivec2 m_offset;
    enum{ COMPUTE1,COMPUTE2,FREE } m_state;
};

#endif // RADIOSITY_H
