#include "Utils/geometryshader.h"

GeometryShader::GeometryShader()
{}

void GeometryShader::createShader()
{
    shader = glCreateShader(GL_GEOMETRY_SHADER);
}

const char *GeometryShader::getShaderTypeStr() const
{
    return "Geometry shader";
}
