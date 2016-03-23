#include "Utils/shaderprogram.h"

ShaderProgram::ShaderProgram()
    : shaderProgram(0)
{}

ShaderProgram::~ShaderProgram()
{}

void ShaderProgram::addVertexShader(VertexShader *vShader)
{
    vShaders.push_back(vShader);
}

void ShaderProgram::addGeometryShader(GeometryShader *gShader)
{
    gShaders.push_back(gShader);
}

void ShaderProgram::addFragmentShader(FragmentShader *fShader)
{
    fShaders.push_back(fShader);
}

void ShaderProgram::addComputeShader(ComputeShader *cShader)
{
    cShaders.push_back(cShader);
}

void ShaderProgram::clearVertexShaders()
{
    vShaders.clear();
}

void ShaderProgram::clearGeometryShaders()
{
    gShaders.clear();
}

void ShaderProgram::clearFragmentShaders()
{
    fShaders.clear();
}

void ShaderProgram::clearComputeShaders()
{
    fShaders.clear();
}

void ShaderProgram::create()
{
    if(!shaderProgram)
        shaderProgram = glCreateProgram();
}

void ShaderProgram::loadCompute(const std::string &csh)
{
    create();
    ComputeShader cShader;
    cShader.init(csh.c_str());

    addComputeShader(&cShader);

    link();

    clearComputeShaders();

    cShader.destroy();
}

void ShaderProgram::loadVertexFragment( const std::string &vsh, const std::string &fsh)
{
    create();
    VertexShader vShader;
    FragmentShader fShader;
    vShader.init( vsh.c_str() );
    fShader.init( fsh.c_str() );

    addVertexShader(&vShader);
    addFragmentShader(&fShader);

    link();

    clearVertexShaders();
    clearFragmentShaders();

    vShader.destroy();
    fShader.destroy();
}

void ShaderProgram::loadVertexFragment(const std::string &vsh, const std::string &gsh, const std::string &fsh)
{
    create();
    VertexShader vShader;
    GeometryShader gShader;
    FragmentShader fShader;
    vShader.init( vsh.c_str() );
    gShader.init( gsh.c_str() );
    fShader.init( fsh.c_str() );

    addVertexShader(&vShader);
    addGeometryShader(&gShader);
    addFragmentShader(&fShader);

    link();

    clearVertexShaders();
    clearGeometryShaders();
    clearFragmentShaders();

    vShader.destroy();
    gShader.destroy();
    fShader.destroy();
}

void ShaderProgram::destroy()
{
    glDeleteProgram(shaderProgram);
    shaderProgram = 0;
}

bool ShaderProgram::link() const
{
    //Check if vertex and fragment are present, geometry is optional
    if(vShaders.size() && fShaders.size())
    {
        for(unsigned i = 0; i < vShaders.size(); ++i)
        {
            glAttachShader(shaderProgram, vShaders.at(i)->getID());
        }
        for(unsigned i = 0; i < gShaders.size(); ++i)
        {
            glAttachShader(shaderProgram, gShaders.at(i)->getID());
        }
        for(unsigned i = 0; i < fShaders.size(); ++i)
        {
            glAttachShader(shaderProgram, fShaders.at(i)->getID());
        }

        glLinkProgram(shaderProgram);

        for(unsigned i = 0; i < vShaders.size(); ++i)
        {
            glDetachShader(shaderProgram, vShaders.at(i)->getID());
        }
        for(unsigned i = 0; i < gShaders.size(); ++i)
        {
            glDetachShader(shaderProgram, gShaders.at(i)->getID());
        }
        for(unsigned i = 0; i < fShaders.size(); ++i)
        {
            glDetachShader(shaderProgram, fShaders.at(i)->getID());
        }
    }
    else
    {
        for(unsigned i = 0; i < cShaders.size(); ++i)
        {
            glAttachShader(shaderProgram, cShaders.at(i)->getID());
        }

        glLinkProgram(shaderProgram);
        
        for(unsigned i = 0; i < cShaders.size(); ++i)
        {
            glDetachShader(shaderProgram, cShaders.at(i)->getID());
        }
    }
    GLint success;

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    GLint length;

    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);

    if(length > 1)
    {
        GLchar *log = new GLchar[length];
        log[0] = 0;

        glGetProgramInfoLog(shaderProgram, length, NULL, log);

        std::cerr << "Shader program logs :"
                  << std::endl << log << std::endl;

        delete []log;
    }

    return success;
}

void ShaderProgram::bind() const
{
    glUseProgram(shaderProgram);
}

void ShaderProgram::unbind()
{
    glUseProgram(0);
}

GLuint ShaderProgram::getID() const
{
    return shaderProgram;
}

void ShaderProgram::sendUniform(const char *name, const unsigned &nbValues, const int *values) const
{
    glUniform1iv(glGetUniformLocation(shaderProgram, name), nbValues, values);
}

void ShaderProgram::sendUniform(const char *name, const unsigned &nbValues, const glm::ivec2 *values) const
{
    glUniform2iv(glGetUniformLocation(shaderProgram, name), nbValues, (int *)values);
}

void ShaderProgram::sendUniform(const char *name, const int &value) const
{
    glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}

void ShaderProgram::sendUniform(const char *name, const glm::ivec2 &value) const
{
    glUniform2iv(glGetUniformLocation(shaderProgram, name), 1, glm::value_ptr(value));
}

void ShaderProgram::sendUniform(const char *name, const unsigned &nbValues, const unsigned *values) const
{
    glUniform1uiv(glGetUniformLocation(shaderProgram, name), nbValues, values);
}

void ShaderProgram::sendUniform(const char *name, const unsigned &value) const
{
    glUniform1ui(glGetUniformLocation(shaderProgram, name), value);
}

void ShaderProgram::sendUniform(const char *name, const unsigned &nbValues, const float *values) const
{
    glUniform1fv(glGetUniformLocation(shaderProgram, name), nbValues, values);
}

void ShaderProgram::sendUniform(const char *name, const float &value) const
{
    glUniform1f(glGetUniformLocation(shaderProgram, name), value);
}

void ShaderProgram::sendUniform(const char *name, const unsigned &nbValues, const glm::vec2 *values) const
{
    glUniform2fv(glGetUniformLocation(shaderProgram, name), nbValues, (float *)values);
}

void ShaderProgram::sendUniform(const char *name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(shaderProgram, name), 1, glm::value_ptr(value));
}

void ShaderProgram::sendUniform(const char *name, const unsigned &nbValues, const glm::vec3 *values) const
{
    glUniform3fv(glGetUniformLocation(shaderProgram, name), nbValues, (float *)values);
}

void ShaderProgram::sendUniform(const char *name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, glm::value_ptr(value));
}

void ShaderProgram::sendUniform(const char *name, const unsigned &nbValues, const glm::mat4 *values) const
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), nbValues, GL_FALSE, (float *)values);
}

void ShaderProgram::sendUniform(const char *name, const glm::mat4 &value) const
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, glm::value_ptr(value));
}
