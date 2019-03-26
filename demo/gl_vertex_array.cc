#include "gl_vertex_array.h"

namespace gl {

vertex_array::vertex_array()
{
    glGenVertexArrays(1, &id_);
}

vertex_array::~vertex_array()
{
    glDeleteVertexArrays(1, &id_);
}

void vertex_array::bind()
{
    glBindVertexArray(id_);
}
}
