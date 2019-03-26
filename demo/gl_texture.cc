#include "gl_texture.h"

namespace gl {

texture::texture(GLenum target, GLsizei width, GLsizei height, GLint internal_format)
    : target_{target}
    , width_{width}
    , height_{height}
    , internal_format_{internal_format}
{
    glGenTextures(1, &id_);
}

texture::~texture()
{
    glDeleteTextures(1, &id_);
}

GLuint texture::id() const
{
    return id_;
}

GLsizei texture::width() const
{
    return width_;
}

GLsizei texture::height() const
{
    return height_;
}

void texture::bind()
{
    glBindTexture(target_, id_);
}

void texture::set_min_filter(GLint filter)
{
    bind();
    glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, filter);
}

void texture::set_mag_filter(GLint filter)
{
    bind();
    glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, filter);
}

void texture::set_wrap_s(GLint wrap)
{
    bind();
    glTexParameteri(target_, GL_TEXTURE_WRAP_S, wrap);
}

void texture::set_wrap_t(GLint wrap)
{
    bind();
    glTexParameteri(target_, GL_TEXTURE_WRAP_T, wrap);
}

void texture::allocate(GLenum format, GLenum type)
{
    bind();
    glTexImage2D(target_, 0, internal_format_, width_, height_, 0, format, type, nullptr);
}

void texture::set_data(GLenum format, GLenum type, const GLvoid *data)
{
    bind();
    glTexSubImage2D(target_, 0, 0, 0, width_, height_, format, type, data);
}

}
