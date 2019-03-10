#include "gl_vertex_array.h"

#include "gl_check.h"

namespace gl {

vertex_array::vertex_array()
{
    GL_CHECK(glGenVertexArrays(1, &id_));
}

vertex_array::~vertex_array()
{
    GL_CHECK(glDeleteVertexArrays(1, &id_));
}

void vertex_array::bind()
{
    GL_CHECK(glBindVertexArray(id_));
}
}
