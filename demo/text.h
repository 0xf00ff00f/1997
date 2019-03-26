#pragma once

#include "effect.h"

#include <GL/glew.h>

#include <memory>
#include <vector>
#include <string_view>

namespace gl
{
class shader_program;
class buffer;
}

class geometry;

class text : public effect
{
public:
    text(int width, int height);
    ~text();

    void redraw(long time) override;

private:
    void init_gl_resources();
    void init_glyph_infos();

    void draw_string(float x, float y, std::string_view str) const;
    int string_width(std::string_view str) const;

    struct glyph_info {
        int width;
        int first_vert;
        int num_verts;
    };
    std::array<std::unique_ptr<glyph_info>, 256> glyph_infos_;
    std::unique_ptr<gl::shader_program> program_;
    std::unique_ptr<geometry> geometry_;
};
