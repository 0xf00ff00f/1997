#include "demo.h"

#include "arrows.h"
#include "gl_check.h"

#include <GL/glew.h>

demo::demo(int width, int height)
    : width_{width}
    , height_{height}
    , effect_{new arrows(width, height)}
{
}

demo::~demo()
{
}

void demo::redraw()
{
    GL_CHECK(glViewport(0, 0, width_, height_));
    GL_CHECK(glClearColor(0.5, 0.5, 0.5, 0.0));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

    effect_->redraw();
}
