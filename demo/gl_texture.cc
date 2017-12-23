#include "gl_texture.h"

#include "gl_check.h"

namespace gl {

texture::texture(GLenum target, GLsizei width, GLsizei height, GLint internal_format)
    : target_{target}
    , width_{width}
    , height_{height}
    , internal_format_{internal_format}
{
    GL_CHECK(glGenTextures(1, &id_));
}

texture::~texture()
{
    GL_CHECK(glDeleteTextures(1, &id_));
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
    GL_CHECK(glBindTexture(target_, id_));
}

void texture::set_min_filter(GLint filter)
{
    bind();
    GL_CHECK(glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, filter));
}

void texture::set_mag_filter(GLint filter)
{
    bind();
    GL_CHECK(glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, filter));
}

void texture::set_wrap_s(GLint wrap)
{
    bind();
    GL_CHECK(glTexParameteri(target_, GL_TEXTURE_WRAP_S, wrap));
}

void texture::set_wrap_t(GLint wrap)
{
    bind();
    GL_CHECK(glTexParameteri(target_, GL_TEXTURE_WRAP_T, wrap));
}

void texture::allocate(GLenum format, GLenum type)
{
    bind();
    GL_CHECK(glTexImage2D(target_, 0, internal_format_, width_, height_, 0, format, type, nullptr));
}

void texture::set_data(GLenum format, GLenum type, const GLvoid *data)
{
    bind();
    GL_CHECK(glTexSubImage2D(target_, 0, 0, 0, width_, height_, format, type, data));
}

}
