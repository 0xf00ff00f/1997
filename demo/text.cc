#include "text.h"

#include "gl_check.h"
#include "gl_shader_program.h"
#include "gl_buffer.h"
#include "vec2.h"

#include <GL/glew.h>

#include <vector>

#include <cstdio>
#include <cassert>

namespace {

constexpr float half_thickness = 8;

using line_strip = std::vector<vec2f>;

const struct {
    int code;
    int width;
    std::vector<line_strip> strips;
} glyphs[] = {
    {
        'a',
        100,
        { { { 0, 88 }, { 88, 88 }, { 88, 8 }, { 8, 8 }, { 8, 16 }, { 88, 80 } } },
    },
    {
        'b',
        100,
        { { { 8, 120 }, { 8, 8 }, { 88, 8 }, { 88, 16 }, { 8, 80 } } },
    },
    {
        'c',
        100,
        { { { 96, 88 }, { 8, 88 }, { 8, 8 }, { 96, 8 } } },
    },
    {
        'd',
        100,
        { { { 88, 120 }, { 88, 8 }, { 8, 8 }, { 8, 16 }, { 88, 80 } } },
    },
    {
        'e',
        100,
        { { { 96, 8 }, { 8, 8 }, { 8, 88 }, { 88, 88 }, { 88, 80 }, { 8, 16 } } },
    },
    {
        'f',
        100,
        { { { 8, -24 }, { 8, 88 }, { 96, 88 } },
          { { 8, 8 }, { 96, 8 } } },
    },
    {
        'g',
        100,
        { { { 88, 8 }, { 8, 8 }, { 8, 88 }, { 88, 88 }, { 88, -24 } } },
    },
    {
        'h',
        100,
        { { { 8, 120 }, { 8, 0 } },
          { { 8, 80 }, { 88, 16 }, { 88, 0 } } },
    },
    {
        'i',
        20,
        { { { 8, 0 }, { 8, 96 } },
          { { 8, 104 }, { 8, 120 } } },
    },
    {
        'j',
        76,
        { { { 64, 96 }, { 64, 16 }, { 24, -16 }, { 0, -16 } },
          { { 64, 104 }, { 64, 120 } } },
    },
    {
        'k',
        100,
        { { { 8, 120 }, { 8, 8 }, { 96, 8 } },
          { { 8, 16 }, { 88, 80 }, { 88, 96 } } },
    },
    {
        'l',
        20,
        { { { 8, 0 }, { 8, 120 } } },
    },
    {
        'm',
        100,
        { { { 8, 0 }, { 8, 88 }, { 88, 88 }, { 88, 0 } },
          { { 48, 88 }, { 48, 0 } } },
    },
    {
        'n',
        100,
        { { { 8, 0 }, { 8, 88 }, { 48, 88 }, { 88, 56 }, { 88, 0 } } },
    },
    {
        'o',
        100,
        { { { 8, 0 }, { 8, 88 }, { 88, 88 }, { 88, 8 }, { 8, 8 } } },
    },
    {
        'p',
        100,
        { { { 8, -24 }, { 8, 88 }, { 88, 88 }, { 88, 8 }, { 8, 8 } } },
    },
    {
        'q',
        100,
        { { { 88, -24 }, { 88, 88 }, { 8, 88 }, { 8, 80 }, { 88, 16 } } },
    },
    {
        'r',
        100,
        { { { 8, 0 }, { 8, 88 }, { 96, 88 } } },
    },
    {
        's',
        100,
        { { { 96, 88 }, { 8, 88 }, { 8, 48 }, { 88, 48 }, { 88, 8 }, { 0, 8 } } },
    },
    {
        't',
        100,
        { { { 8, 120 }, { 8, 8 }, { 96, 8 } },
          { { 8, 88 }, { 96, 88 } } },
    },
    {
        'u',
        100,
        { { { 8, 96 }, { 8, 8 }, { 88, 8 }, { 88, 96 } } }
    },
    {
        'v',
        100,
        { { { 8, 96 }, { 8, 40 }, { 48, 8 }, { 88, 8 }, { 88, 96 } } },
    },
    {
        'w',
        100,
        { { { 8, 96 }, { 8, 8 }, { 88, 8 }, { 88, 96 } },
          { { 48, 8 }, { 48, 96 } } },
    },
    {
        'x',
        100,
        { { { 8, 96 }, { 8, 80 }, { 88, 16 }, { 88, 0 } },
          { { 88, 96 }, { 88, 80 }, { 8, 16 }, { 8, 0 } } },
    },
    {
        'y',
        100,
        { { { 8, 96 }, { 8, 80 }, { 88, 16 } },
          { { 88, 96 }, { 88, -24 } } },
    },
    {
        'z',
        100,
        { { { 0, 88 }, { 88, 88 }, { 88, 80 }, { 8, 16 }, { 8, 8 }, { 96, 8 } } },
    },
};

std::vector<vec2f> triangle_strip_from_line_strip(const line_strip& verts)
{
    assert(verts.size() >= 2);

    std::vector<vec2f> tri_strip;

    // first

    {
        const auto& p0 = verts[0];
        const auto& p1 = verts[1];

        const vec2f d = normalized(p1 - p0);
        const vec2f n = vec2f(-d.y, d.x);
        const vec2f u = n*half_thickness;

        tri_strip.push_back(p0 - u);
        tri_strip.push_back(p0 + u);
    }

    for (size_t i = 1; i < verts.size() - 1; ++i) {
        const auto& p0 = verts[i - 1];
        const auto& p1 = verts[i];
        const auto& p2 = verts[i + 1];

        vec2f d0 = normalized(p1 - p0);
        vec2f n0 = vec2f(-d0.y, d0.x);

        vec2f d1 = normalized(p2 - p1);
        vec2f n1 = vec2f(-d1.y, d1.x);

        vec2f n = normalized(n0 + n1);

        float l = half_thickness/dot(n, n0);

        vec2f u = n*l;

        tri_strip.push_back(p1 - u);
        tri_strip.push_back(p1 + u);
    }

    // last

    {
        const auto& p0 = verts[verts.size() - 1];
        const auto& p1 = verts[verts.size() - 2];

        const vec2f d = normalized(p0 - p1);
        const vec2f n = vec2f(-d.y, d.x);
        const vec2f u = n*half_thickness;

        tri_strip.push_back(p0 - u);
        tri_strip.push_back(p0 + u);
    }

    return tri_strip;
}

}

text::text(int width, int height)
    : effect{width, height}
    , program_{new gl::shader_program}
{
    init_gl_resources();
    init_glyph_infos();
}

text::~text()
{
}

void text::redraw(unsigned)
{
    draw_string(32, 520, "flare");
    draw_string(32, 360, "nineteen ninety seven");
    draw_string(32, 200, "greetz lorem ipsum");
    draw_string(32, 40, "the quick brown fox");
}

void text::init_gl_resources()
{
    gl::shader vert_shader(GL_VERTEX_SHADER);
    vert_shader.compile_source_file("assets/text.vert");
    program_->add_shader(vert_shader);

    gl::shader frag_shader(GL_FRAGMENT_SHADER);
    frag_shader.compile_source_file("assets/text.frag");
    program_->add_shader(frag_shader);

    program_->link();
}

void text::init_glyph_infos()
{
    for (const auto& glyph : glyphs) {
        std::vector<GLfloat> verts;

        for (auto& line_strip : glyph.strips) {
            auto s = triangle_strip_from_line_strip(line_strip);

            if (!verts.empty()) {
                // add degenerate triangle
                auto prev_x = verts[verts.size() - 2];
                auto prev_y = verts[verts.size() - 1];
                verts.push_back(prev_x);
                verts.push_back(prev_y);

                verts.push_back(s.front().x);
                verts.push_back(s.front().y);
            }

            for (const auto& v : s) {
                verts.push_back(v.x);
                verts.push_back(v.y);
            }
        }

        std::unique_ptr<gl::buffer> vbo{new gl::buffer(GL_ARRAY_BUFFER)};
        vbo->set_data(verts.size()*sizeof(GLfloat), &verts[0]);

        std::unique_ptr<glyph_info> gi{new glyph_info};
        gi->width = glyph.width;
        gi->num_verts = verts.size()/2;
        gi->vbo = std::move(vbo);

        glyph_infos_[glyph.code] = std::move(gi);
    }
}

std::array<GLfloat, 16> text::glyph_mvp(float x, float y) const
{
    const float virt_width = 2275;
    const float virt_height = 1280;

    const GLfloat a = 2.f/virt_width;
    const GLfloat b = 2.f/virt_height;

    return { a, 0, 0, a*x - 1,
             0, b, 0, b*y - 1,
             0, 0, 1, 0,
             0, 0, 0, 1 };
}

void text::draw_string(float x, float y, const char *str) const
{
    program_->bind();

    for (const char *p = str; *p; ++p) {
        char ch = *p;
        auto& gi = glyph_infos_[ch];
        if (gi) {
            program_->set_uniform_matrix4("proj_modelview", glyph_mvp(x, y));

            gi->vbo->bind();
            GL_CHECK(glEnableVertexAttribArray(0));
            GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0)));
            GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, gi->num_verts));

            x += gi->width;
        } else {
            x += 100;
        }
    }
}

