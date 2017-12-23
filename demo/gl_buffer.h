#pragma once

#include "noncopyable.h"

#include <GL/glew.h>

namespace gl {

class buffer : private noncopyable
{
public:
    buffer(GLenum target);
    ~buffer();

    void bind();
    void set_data(GLsizeiptr size, const GLvoid *data, GLenum usage = GL_STATIC_DRAW);

private:
    GLuint id_;
    GLenum target_;
};

} // gl
