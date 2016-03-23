#include "Utils/computeshader.h"

ComputeShader::ComputeShader()
{}

void ComputeShader::createShader()
{
    shader = glCreateShader(GL_COMPUTE_SHADER);
}

const char *ComputeShader::getShaderTypeStr() const
{
	return "Compute shader";
}
