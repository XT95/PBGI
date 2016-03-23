#include "Utils/vertexshader.h"

VertexShader::VertexShader()
{}

void VertexShader::createShader()
{
    shader = glCreateShader(GL_VERTEX_SHADER);
}

const char *VertexShader::getShaderTypeStr() const
{
	return "Vertex shader";
}
