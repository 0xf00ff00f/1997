#include "gl_buffer.h"

namespace gl {

buffer::buffer(GLenum target)
    : target_{target}
{
    glGenBuffers(1, &id_);
}

buffer::~buffer()
{
    glDeleteBuffers(1, &id_);
}

void buffer::bind()
{
    glBindBuffer(target_, id_);
}

void buffer::set_data(GLsizeiptr size, const GLvoid *data, GLenum usage)
{
    bind();
    glBufferData(target_, size, data, usage);
}

} // gl
