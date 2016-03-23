#ifndef FRAGMENTSHADER_H
#define FRAGMENTSHADER_H

#include "Utils/abstractshader.h"

class FragmentShader : public AbstractShader
{
public:
    FragmentShader();

protected:
	void createShader();
	const char *getShaderTypeStr() const;
};

#endif // FRAGMENTSHADER_H
