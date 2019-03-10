#pragma once

#include "noncopyable.h"

#include <GL/glew.h>

namespace gl {

class vertex_array : private noncopyable
{
public:
    vertex_array();
    ~vertex_array();

    void bind();

private:
    GLuint id_;
};

}
