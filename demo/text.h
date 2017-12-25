#pragma once

#include "effect.h"

#include <GL/glew.h>

#include <memory>
#include <vector>

namespace gl {
class shader_program;
class buffer;
}

class text : public effect
{
public:
    text(int width, int height);
    ~text();

    void redraw(unsigned time) override;

private:
    void init_gl_resources();
    void init_glyph_infos();

    void draw_string(float x, float y, const char *str) const;
    std::array<GLfloat, 16> glyph_mvp(float x, float y) const;

    struct glyph_info {
        int width;
        int num_verts;
        std::unique_ptr<gl::buffer> vbo;
    };
    std::array<std::unique_ptr<glyph_info>, 256> glyph_infos_;
    std::unique_ptr<gl::shader_program> program_;
};
