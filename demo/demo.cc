#include "demo.h"

#include "arrows.h"
#include "text.h"
#include "gl_check.h"

#include <GL/glew.h>

demo::demo(int width, int height)
    : width_{width}
    , height_{height}
    , arrow_effect_{new arrows(width, height)}
    , text_effect_{new text(width, height)}
{
    start_ = std::chrono::steady_clock::now();
}

demo::~demo()
{
}

void demo::redraw()
{
    const auto now = std::chrono::steady_clock::now();

    GL_CHECK(glViewport(0, 0, width_, height_));
    GL_CHECK(glClearColor(0.5, 0.5, 0.5, 0.0));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_).count();
    arrow_effect_->redraw(elapsed);
    text_effect_->redraw(elapsed);
}
