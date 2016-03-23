#ifndef OBJECT_H
#define OBJECT_H

#include <tiny_obj_loader.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Utils/shaderprogram.h"
#include "Engine/texture.h"

class Object
{
public:
    Object();
    ~Object();

    void load(const std::string &filename , const std::string &path);
    void updateGPUBuffers();

    void setTransform( const glm::mat4 &m ) { m_mat = m; }
    glm::mat4& getTransform() { return m_mat; }

    void setTexture( const std::string &filename );
    void setTexture( Texture * tex);
    Texture* getTexture() { return m_tex; }

    unsigned getNbVertex() { return m_nbVertex; }
    unsigned getNbFaces() { return m_nbFaces; }
    glm::vec3* getVertex() { return m_vertex; }
    glm::vec3* getNormals() { return m_normals; }
    glm::vec3* getNormalsFaces() { return m_normalsFaces; }
    glm::vec2* getTexCoords() { return m_uv; }
    glm::vec2* getTexCoordsLightmap() { return m_uvlightmap; }
    float*     getAreaFaces() { return m_areaFaces; }

    void draw();

    void drawMaterials(const ShaderProgram &);
private:
    glm::mat4 m_mat;
    std::vector<tinyobj::material_t> m_materials;
    std::vector<glm::uvec3> materialIndices;
    glm::vec3 *m_vertex;
    glm::vec3 *m_normals;
    glm::vec3 *m_normalsFaces;
    glm::vec2 *m_uv;
    glm::vec2 *m_uvlightmap;
    float     *m_areaFaces;
    Texture *m_tex;
    GLuint m_vertexArray;
    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;

    unsigned m_nbVertex;
    unsigned m_nbIndices;
    unsigned m_nbFaces;
};

#endif // OBJECT_H
