#include "gl_buffer.h"

#include "gl_check.h"

namespace gl {

buffer::buffer(GLenum target)
    : target_{target}
{
    GL_CHECK(glGenBuffers(1, &id_));
}

buffer::~buffer()
{
    GL_CHECK(glDeleteBuffers(1, &id_));
}

void buffer::bind()
{
    GL_CHECK(glBindBuffer(target_, id_));
}

void buffer::set_data(GLsizeiptr size, const GLvoid *data, GLenum usage)
{
    bind();
    GL_CHECK(glBufferData(target_, size, data, usage));
}

} // gl
