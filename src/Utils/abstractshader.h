#ifndef ABSTRACTSHADER_H
#define ABSTRACTSHADER_H

#include <GL/glew.h>

#include "Utils/loadfile.h"

class AbstractShader
{
public:
	AbstractShader();

	void create();
	bool init(const char *);

	bool compile(const char *);
	bool compileSources(const char *);
	bool compile() const;

	void destroy();

	GLuint getID() const;
	
protected:
	virtual void createShader() = 0;
	virtual const char *getShaderTypeStr() const = 0;
	GLuint shader;

private:
	std::string sources;
	bool isCreated;
};

#endif // ABSTRACTSHADER_H
