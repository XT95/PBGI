#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include "Utils/abstractshader.h"

class VertexShader : public AbstractShader
{
public:
    VertexShader();

protected:
    void createShader();
	const char *getShaderTypeStr() const;
};

#endif // VERTEXSHADER_H
