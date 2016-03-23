#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Engine/texture.h"

Texture::Texture()
{
    glGenTextures(1, &m_id);
    m_width = 0;
    m_height = 0;
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

void Texture::bind(const int &channel)
{
    glActiveTexture(GL_TEXTURE0+channel);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::bindAsImage(const int &channel, const GLenum &mode)
{
    glBindImageTexture(channel, m_id, 0, GL_FALSE, 0, mode, GL_RGBA32F);
}

void Texture::load(const std::string &filename, const bool &gammaCorrected)
{
    int comp;
    unsigned char *data = stbi_load(filename.c_str(), &m_width, &m_height, &comp, 0);

    if(!data)
    {
        std::cout << "Failed to load texture" << filename << std::endl;
        return;
    }

    glBindTexture(GL_TEXTURE_2D, m_id);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE); // Obselet
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if(comp == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, m_format = gammaCorrected ? GL_SRGB : GL_RGB, m_width,m_height, 0, GL_RGB,GL_UNSIGNED_BYTE, data);
    else if(comp == 4)
        glTexImage2D(GL_TEXTURE_2D, 0,  m_format = gammaCorrected ? GL_SRGB_ALPHA : GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

void Texture::load(const void *data, const int &width, const int &height, GLenum intf, GLenum f, GLenum type )
{
    m_width = width;
    m_height = height;

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, intf, m_width, m_height, 0, f, type, (const GLvoid*)data);

   // glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setFiltering(const GLenum &filtering)
{
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setFormat(const GLenum &format)
{
    m_format = format;
}

void Texture::setSize(const int &width, const int &height)
{
    m_width = width;
    m_height = height;
}

void Texture::updateTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}
