#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <iostream>



class Texture
{
public:
    Texture();
    ~Texture();

    void load(const std::string &path, const bool &gammaCorrected = true);
    void load(const void *data, const int &width, const int &height, GLenum intf, GLenum f , GLenum type);

    void bind(const int &channel);
    void bindAsImage(const int &channel, const GLenum &mode);

    void updateTexture();

    void setSize(const int &width, const int &height);
    void setFormat(const GLenum &format);

    void setFiltering(const GLenum &filtering);
    void setWrapping(const GLenum &wrapping);

    int  getWidth(){return m_width;}
    int  getHeight(){return m_height;}
    GLuint getID() {return m_id;}
protected:
    GLuint m_id;
    GLenum m_format;
    int    m_width, m_height;
};

#endif // TEXTURE_H
