#include "Utils/abstractshader.h"

AbstractShader::AbstractShader()
	: shader(0)
{}

bool AbstractShader::init(const char *name)
{
	create();
	
	if(!loadFile(name, sources))
		return false;
	
	return compile();
}

void AbstractShader::create()
{
    if(!shader)
        createShader();
}

bool AbstractShader::compile(const char *name)
{
	if(!loadFile(name, sources))
		return false;

	return compile();
}

bool AbstractShader::compileSources(const char *shaderSources)
{
	sources.clear();

	sources = std::string(shaderSources);

	return compile();
}

bool AbstractShader::compile() const
{
	const char *src = sources.c_str();
    glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

    GLint success;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    GLint length;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    
    if(length > 1)
    {
        GLchar *log = new GLchar[length];
        log[0] = 0;

        glGetShaderInfoLog(shader, length, NULL, log);

        std::cerr << getShaderTypeStr() << " log :"
                   << std::endl << log << std::endl;

        delete []log;
    }

    return success;
}

void AbstractShader::destroy()
{
	glDeleteShader(shader);
	shader = 0;
}

GLuint AbstractShader::getID() const
{
	return shader;
}
