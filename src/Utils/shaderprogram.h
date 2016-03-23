#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Utils/vertexshader.h"
#include "Utils/geometryshader.h"
#include "Utils/fragmentshader.h"
#include "Utils/computeshader.h"

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    void addVertexShader(VertexShader *);
    void addGeometryShader(GeometryShader *);
    void addFragmentShader(FragmentShader *);
    void addComputeShader(ComputeShader *);

    void clearVertexShaders();
    void clearGeometryShaders();
    void clearFragmentShaders();
    void clearComputeShaders();

    void loadCompute(const std::string &csh);
    void loadVertexFragment(const std::string &vsh, const std::string &fsh);
    void loadVertexFragment(const std::string &vsh,  const std::string &gsh, const std::string &fsh);

    void create();

    void destroy();

	bool link() const;
	
	void bind() const;
	static void unbind();
	
	GLuint getID() const;

	// Méthodes permettant d'envoyer une valeur uniforme au shader
	void sendUniform(const char *, const unsigned &, const int *) const;
    void sendUniform(const char *, const unsigned &, const glm::ivec2 *) const;
	void sendUniform(const char *, const unsigned &, const unsigned *) const;
	void sendUniform(const char *, const unsigned &, const float *) const;
	void sendUniform(const char *, const unsigned &, const glm::vec2 *) const;
	void sendUniform(const char *, const unsigned &, const glm::vec3 *) const;
	void sendUniform(const char *, const unsigned &, const glm::mat4 *) const;
	
	void sendUniform(const char *, const int &) const;
    void sendUniform(const char *, const glm::ivec2 &) const;
	void sendUniform(const char *, const unsigned &) const;
	void sendUniform(const char *, const float &) const;
	void sendUniform(const char *, const glm::vec2 &) const;
	void sendUniform(const char *, const glm::vec3 &) const;
    void sendUniform(const char *, const glm::mat4 &) const;
private:
    std::vector<VertexShader *> vShaders;
    std::vector<GeometryShader *> gShaders;
    std::vector<FragmentShader *> fShaders;
    std::vector<ComputeShader *> cShaders;
	GLuint shaderProgram;
	bool isCreated;
};

#endif // SHADERPROGRAM_H
