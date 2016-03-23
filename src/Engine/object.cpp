#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc

#include <iostream>
#include "Engine/object.h"
#include "Utils/utils.h"

#define BUFFER_OFFSET(i) ((void*)(i))

using namespace std;


Object::Object()
{
    m_vertex = NULL;
    m_normals = NULL;
    m_normalsFaces = NULL;
    m_uv = NULL;
    m_uvlightmap = NULL;
    m_areaFaces = NULL;
    m_tex = NULL;
    m_mat = glm::mat4(1.f);

    m_nbVertex = 0;
    m_nbFaces = 0;
    m_nbIndices = 0;
}

Object::~Object()
{
    delete[] m_vertex;
    delete[] m_normals;
    delete[] m_normalsFaces;
    delete[] m_uv;
    delete[] m_uvlightmap;
    delete[] m_areaFaces;
    delete m_tex;
}

void Object::setTexture(const string &filename)
{
    if(m_tex)
        delete m_tex;
    m_tex = new Texture();
    m_tex->load(filename);
}

void Object::setTexture(Texture *tex)
{
    m_tex = tex;
}

void Object::load(const std::string &filename, const std::string &path )
{
   std::vector<tinyobj::shape_t> m_shapes;

    std::string err;
    bool ret = tinyobj::LoadObj(m_shapes, m_materials, err, filename.c_str(), path.c_str(), false);

    if (!err.empty()) { // `err` may contain warning message.
        std::cerr << err << std::endl;
    }

    if (!ret) {
        exit(1);
    }

    m_nbVertex = 0;
    for (size_t i = 0; i < m_shapes.size(); i++)
        m_nbVertex += m_shapes[i].mesh.indices.size();
    m_nbFaces = m_nbVertex/4;

    m_nbIndices = m_nbVertex+m_nbVertex/2;

    int k = 0;
    int l = 0;
    m_vertex = new glm::vec3[m_nbVertex];
    m_normals = new glm::vec3[m_nbVertex];
    m_normalsFaces = new glm::vec3[m_nbFaces];
    m_uv = new glm::vec2[m_nbVertex];
    m_uvlightmap = new glm::vec2[m_nbVertex];
    m_areaFaces = new float[m_nbFaces];

    unsigned prevMaterialID = m_shapes[0].mesh.material_ids[0], currentMaterialID;
    unsigned beginingMaterial = 0;
    unsigned cumulatedVertex = 0;
    for(size_t i=0; i<m_shapes.size(); i++)
    {
        for (size_t f = 0; f < m_shapes[i].mesh.indices.size(); f+=4)
        {

            //Compute face info
            glm::vec3 a = glm::vec3(m_shapes[i].mesh.positions[ 3*m_shapes[i].mesh.indices[f+0]+0 ],
                    m_shapes[i].mesh.positions[ 3*m_shapes[i].mesh.indices[f+0]+1 ],
                    m_shapes[i].mesh.positions[ 3*m_shapes[i].mesh.indices[f+0]+2 ]);
            glm::vec3 b = glm::vec3(m_shapes[i].mesh.positions[ 3*m_shapes[i].mesh.indices[f+1]+0 ],
                    m_shapes[i].mesh.positions[ 3*m_shapes[i].mesh.indices[f+1]+1 ],
                    m_shapes[i].mesh.positions[ 3*m_shapes[i].mesh.indices[f+1]+2 ]);
            glm::vec3 c = glm::vec3(m_shapes[i].mesh.positions[ 3*m_shapes[i].mesh.indices[f+2]+0 ],
                    m_shapes[i].mesh.positions[ 3*m_shapes[i].mesh.indices[f+2]+1 ],
                    m_shapes[i].mesh.positions[ 3*m_shapes[i].mesh.indices[f+2]+2 ]);
            glm::vec3 d = glm::vec3(m_shapes[i].mesh.positions[ 3*m_shapes[i].mesh.indices[f+3]+0 ],
                    m_shapes[i].mesh.positions[ 3*m_shapes[i].mesh.indices[f+3]+1 ],
                    m_shapes[i].mesh.positions[ 3*m_shapes[i].mesh.indices[f+3]+2 ]);

            float aa = glm::sqrt(glm::dot(glm::normalize(b-a), glm::normalize(d-a)));
            float ac = glm::sqrt(glm::dot(glm::normalize(b-c), glm::normalize(d-c)));

            float la = glm::length(b-a);
            float lb = glm::length(b-c);
            float lc = glm::length(c-d);
            float ld = glm::length(d-a);
            float s = .5*(la+lb+lc+ld);
            m_areaFaces[l] = glm::sqrt((s-la)*(s-lb)*(s-lc)*(s-ld)-la*lb*lc*ld*glm::cos(.5f*aa+ac));
            m_normalsFaces[l] = glm::normalize( glm::cross( a-b, a-c) );
            l++;

            currentMaterialID = m_shapes[i].mesh.material_ids.at(f/4);
            if(currentMaterialID != prevMaterialID)
            {
                materialIndices.push_back(glm::uvec3(beginingMaterial, cumulatedVertex, prevMaterialID));
                beginingMaterial = cumulatedVertex;
                prevMaterialID = currentMaterialID;
            }
            cumulatedVertex += 4;
            //Compute per vertex info
            for(int m=0; m<4; m++)
            {
                int id = m_shapes[i].mesh.indices[f+m];
                for(int j=0; j<3; j++)
                {
                    m_vertex[k][j] = m_shapes[i].mesh.positions[ 3*id+j ];
                    m_normals[k][j] = m_shapes[i].mesh.normals[ 3*id+j ];
                    if(j<2)
                    {
                        m_uv[k][j] = m_shapes[i].mesh.texcoords[ 2*id+j ];
                        m_uvlightmap[k][j] = 0.f;
                    }
                }
                k++;
            }
        }
    }

    materialIndices.push_back(glm::uvec3(beginingMaterial, cumulatedVertex, currentMaterialID));

    glGenVertexArrays(1, &m_vertexArray);
    glGenBuffers(1, &m_vertexBuffer);
    glGenBuffers(1, &m_indexBuffer);
    updateGPUBuffers();

}

void Object::updateGPUBuffers()
{
    glBindVertexArray(m_vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_nbVertex*(3+3+2+2), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER,                                0, sizeof(glm::vec3)*m_nbVertex, m_vertex);
    glBufferSubData(GL_ARRAY_BUFFER,       sizeof(float)*m_nbVertex*3, sizeof(glm::vec3)*m_nbVertex, m_normals);
    glBufferSubData(GL_ARRAY_BUFFER,   sizeof(float)*m_nbVertex*(3+3), sizeof(glm::vec2)*m_nbVertex, m_uv);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*m_nbVertex*(3+3+2), sizeof(glm::vec2)*m_nbVertex, m_uvlightmap);

    GLuint *indices = new GLuint[m_nbIndices];

    for(unsigned i = 0; i < m_nbIndices/6; i++)
    {
        indices[i*6+0] = i*4;
        indices[i*6+1] = i*4+1;
        indices[i*6+2] = i*4+3;
        indices[i*6+3] = i*4+3;
        indices[i*6+4] = i*4+1;
        indices[i*6+5] = i*4+2;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_nbIndices, indices, GL_STATIC_DRAW);

    delete []indices;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float)*m_nbVertex*3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float)*m_nbVertex*(3+3)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float)*m_nbVertex*(3+3+2)));

    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Object::draw()
{
    if(m_tex)
        m_tex->bind(0);
    glBindVertexArray(m_vertexArray);
    glDrawElements(GL_TRIANGLES, m_nbIndices, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

void Object::drawMaterials(const ShaderProgram &program)
{
    if(m_tex)
        m_tex->bind(0);
    glBindVertexArray(m_vertexArray);

    for(unsigned i = 0; i < materialIndices.size(); ++i)
    {
        glm::uvec3 indices = materialIndices.at(i);

        glm::vec3 ambientColor = glm::vec3(m_materials.at(indices.z).ambient[0],
                                           m_materials.at(indices.z).ambient[1],
                                           m_materials.at(indices.z).ambient[2]);
        glm::vec3 diffuseColor = glm::vec3(m_materials.at(indices.z).diffuse[0],
                                           m_materials.at(indices.z).diffuse[1],
                                           m_materials.at(indices.z).diffuse[2]);

        program.sendUniform("ambientColor", ambientColor);
        program.sendUniform("diffuseColor", diffuseColor);
        program.sendUniform("modelMatrix", m_mat);
        glDrawElementsBaseVertex(GL_TRIANGLES, (indices.y-indices.x)/4*6, GL_UNSIGNED_INT, NULL, indices.x);
    }

    glBindVertexArray(0);
}
