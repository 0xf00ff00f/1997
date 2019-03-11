#pragma once

#include "effect.h"

#include <GL/glew.h>

#include <memory>
#include <vector>

namespace gl {
class shader_program;
class buffer;
class vertex_array;
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
    std::array<GLfloat, 16> mvp(float x, float y) const;
    int string_width(const char *str) const;

    struct glyph_info {
        int width;
        int first_vert;
        int num_verts;
    };
    std::array<std::unique_ptr<glyph_info>, 256> glyph_infos_;
    std::unique_ptr<gl::shader_program> program_;
    std::unique_ptr<gl::buffer> vbo_;
    std::unique_ptr<gl::vertex_array> vao_;
};
