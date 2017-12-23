#include "demo.h"

#include "arrows.h"
#include "gl_check.h"

#include <GL/glew.h>

#include <time.h>

namespace {

uint64_t get_cur_ms()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec*1000ull + ts.tv_nsec/1000000;
}

}

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
    uint64_t cur_ms = get_cur_ms();
    if (start_ms_ == static_cast<uint64_t>(-1))
        start_ms_ = cur_ms;

    GL_CHECK(glViewport(0, 0, width_, height_));
    GL_CHECK(glClearColor(0.5, 0.5, 0.5, 0.0));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

    effect_->redraw(cur_ms - start_ms_);
}
