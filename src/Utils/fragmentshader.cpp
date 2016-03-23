#include "Utils/fragmentshader.h"

FragmentShader::FragmentShader()
{}

void FragmentShader::createShader()
{
    shader = glCreateShader(GL_FRAGMENT_SHADER);
}

const char *FragmentShader::getShaderTypeStr() const
{
	return "Fragment shader";
}
