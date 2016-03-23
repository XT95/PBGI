#ifndef COMPUTESHADER_H
#define COMPUTESHADER_H

#include "Utils/abstractshader.h"

class ComputeShader : public AbstractShader
{
public:
    ComputeShader();

protected:
    void createShader();
	const char *getShaderTypeStr() const;
};

#endif // COMPUTESHADER_H
