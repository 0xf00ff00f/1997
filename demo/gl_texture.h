#pragma once

#include "noncopyable.h"

#include <GL/glew.h>

namespace gl
{

class texture : private noncopyable
{
public:
    texture(GLenum target, GLsizei width, GLsizei height, GLint internal_format);
    ~texture();

    GLuint id() const;

    GLsizei width() const;
    GLsizei height() const;

    void bind();

    void set_min_filter(GLint filter);
    void set_mag_filter(GLint filter);

    void set_wrap_s(GLint wrap);
    void set_wrap_t(GLint wrap);

    void allocate(GLenum format, GLenum type);
    void set_data(GLenum format, GLenum type, const GLvoid *data);

private:
    GLuint id_;
    GLenum target_;
    GLsizei width_;
    GLsizei height_;
    GLint internal_format_;
};

} // namespace gl
