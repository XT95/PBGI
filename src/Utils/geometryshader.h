#ifndef GEOMETRYSHADER_H
#define GEOMETRYSHADER_H

#include "Utils/abstractshader.h"

class GeometryShader : public AbstractShader
{
public:
    GeometryShader();

protected:
	void createShader();
	const char *getShaderTypeStr() const;
};

#endif // GEOMETRYSHADER_H
