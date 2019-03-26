#include "text.h"

#include "geometry.h"
#include "gl_shader_program.h"
#include "vec2.h"

#include <GL/glew.h>

#include <vector>

#include <cstdio>
#include <cstddef>
#include <cassert>

namespace {

constexpr float aspect_ratio = 16.0/9.0;

constexpr float virt_height = 640;
constexpr float virt_width = virt_height*aspect_ratio;

constexpr float half_thickness = 8;

struct line_strip {
    bool closed;
    std::vector<vec2f> verts;
};

const struct {
    int code;
    int width;
    std::vector<line_strip> strips;
} glyphs[] {
    {
        'a',
        100,
        { { false, { { 0, 88 }, { 88, 88 }, { 88, 8 }, { 8, 8 }, { 8, 16 }, { 88, 80 } } } },
    },
    {
        'b',
        100,
        { { false, { { 8, 120 }, { 8, 8 }, { 88, 8 }, { 88, 16 }, { 8, 80 } } } },
    },
    {
        'c',
        100,
        { { false, { { 96, 88 }, { 8, 88 }, { 8, 8 }, { 96, 8 } } } },
    },
    {
        'd',
        100,
        { { false, { { 88, 120 }, { 88, 8 }, { 8, 8 }, { 8, 16 }, { 88, 80 } } } },
    },
    {
        'e',
        100,
        { { false, { { 96, 8 }, { 8, 8 }, { 8, 88 }, { 88, 88 }, { 88, 80 }, { 8, 16 } } } },
    },
    {
        'f',
        100,
        { { false, { { 8, -24 }, { 8, 88 }, { 96, 88 } } },
          { false, { { 8, 8 }, { 96, 8 } } } },
    },
    {
        'g',
        100,
        { { false, { { 88, 8 }, { 8, 8 }, { 8, 88 }, { 88, 88 }, { 88, -24 } } } },
    },
    {
        'h',
        100,
        { { false, { { 8, 120 }, { 8, 0 } } },
          { false, { { 8, 80 }, { 88, 16 }, { 88, 0 } } } },
    },
    {
        'i',
        20,
        { { false, { { 8, 0 }, { 8, 96 } } },
          { false, { { 8, 104 }, { 8, 120 } } } },
    },
    {
        'j',
        76,
        { { false, { { 64, 96 }, { 64, 16 }, { 24, -16 }, { 0, -16 } } },
          { false, { { 64, 104 }, { 64, 120 } } } },
    },
    {
        'k',
        100,
        { { false, { { 8, 120 }, { 8, 8 }, { 96, 8 } } },
          { false, { { 8, 16 }, { 88, 80 }, { 88, 96 } } } },
    },
    {
        'l',
        20,
        { { false, { { 8, 0 }, { 8, 120 } } } },
    },
    {
        'm',
        100,
        { { false, { { 8, 0 }, { 8, 88 }, { 88, 88 }, { 88, 0 } } },
          { false, { { 48, 88 }, { 48, 0 } } } },
    },
    {
        'n',
        100,
        { { false, { { 8, 0 }, { 8, 88 }, { 48, 88 }, { 88, 56 }, { 88, 0 } } } },
    },
    {
        'o',
        100,
        { { true, { { 8, 8 }, { 8, 88 }, { 88, 88 }, { 88, 8 } } } },
    },
    {
        'p',
        100,
        { { false, { { 8, -24 }, { 8, 88 }, { 88, 88 }, { 88, 8 }, { 8, 8 } } } },
    },
    {
        'q',
        100,
        { { false, { { 88, -24 }, { 88, 88 }, { 8, 88 }, { 8, 80 }, { 88, 16 } } } },
    },
    {
        'r',
        100,
        { { false, { { 8, 0 }, { 8, 88 }, { 96, 88 } } } },
    },
    {
        's',
        100,
        { { false, { { 96, 88 }, { 8, 88 }, { 8, 48 }, { 88, 48 }, { 88, 8 }, { 0, 8 } } } },
    },
    {
        't',
        100,
        { { false, { { 8, 120 }, { 8, 8 }, { 96, 8 } } },
          { false, { { 8, 88 }, { 96, 88 } } } },
    },
    {
        'u',
        100,
        { { false, { { 8, 96 }, { 8, 8 }, { 88, 8 }, { 88, 96 } } } }
    },
    {
        'v',
        100,
        { { false, { { 8, 96 }, { 8, 40 }, { 48, 8 }, { 88, 8 }, { 88, 96 } } } },
    },
    {
        'w',
        100,
        { { false, { { 8, 96 }, { 8, 8 }, { 88, 8 }, { 88, 96 } } },
          { false, { { 48, 8 }, { 48, 96 } } } },
    },
    {
        'x',
        100,
        { { false, { { 8, 96 }, { 8, 80 }, { 88, 16 }, { 88, 0 } } },
          { false, { { 88, 96 }, { 88, 80 }, { 8, 16 }, { 8, 0 } } } },
    },
    {
        'y',
        100,
        { { false, { { 8, 96 }, { 8, 80 }, { 88, 16 } } },
          { false, { { 88, 96 }, { 88, -24 } } } },
    },
    {
        'z',
        100,
        { { false, { { 0, 88 }, { 88, 88 }, { 88, 80 }, { 8, 16 }, { 8, 8 }, { 96, 8 } } } },
    },
};

auto triangle_strip_from_line_strip(const line_strip& strip)
{
    const auto& verts = strip.verts;

    assert(verts.size() >= 2);

    std::vector<std::pair<vec2f, vec2f>> tri_strip;

    const auto num_verts = verts.size();
    const auto count = strip.closed ? num_verts + 1 : num_verts;

    for (size_t i = 0; i < count; ++i) {
        const auto& p = verts[i%num_verts];

        vec2f u;

        if (i == 0 && !strip.closed) {
            // first
            const auto& p1 = verts[1];

            const vec2f d = normalized(p1 - p);
            const vec2f n = vec2f(-d.y, d.x);

            u = n*half_thickness;
        } else if (i == verts.size() - 1 && !strip.closed) {
            // last

            const auto& p1 = verts[i - 1];

            const vec2f d = normalized(p - p1);
            const vec2f n = vec2f(-d.y, d.x);

            u = n*half_thickness;
        } else {
            const auto& p0 = verts[(i + num_verts - 1)%num_verts];
            const auto& p2 = verts[(i + 1)%num_verts];

            vec2f d0 = normalized(p - p0);
            vec2f n0 = vec2f(-d0.y, d0.x);

            vec2f d1 = normalized(p2 - p);
            vec2f n1 = vec2f(-d1.y, d1.x);

            vec2f n = normalized(n0 + n1);

            float l = half_thickness/dot(n, n0);

            u = n*l;
        }

        tri_strip.emplace_back(p - u, p + u);
    }

    return tri_strip;
}

}

text::text(int width, int height)
    : effect{width, height}
    , program_{new gl::shader_program}
    , geometry_{new geometry}
{
    init_gl_resources();
    init_glyph_infos();
}

text::~text()
{
}

void text::redraw(long)
{
    const std::array<const char *, 4> text { "flare", "nineteen", "ninety", "seven" };

    constexpr int line_height = 124;
    float y = .5*(virt_height + line_height*(text.size() - 1));

    for (const auto line : text) {
        draw_string(virt_width - 32 - string_width(line), y, line);
        y -= line_height;
    }
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
    struct vert
    {
        GLfloat x;
        GLfloat y;
        GLfloat d;
    };
    std::vector<vert> verts;

    for (const auto& glyph : glyphs) {
        std::unique_ptr<glyph_info> gi{new glyph_info};
        gi->width = glyph.width;
        gi->first_vert = verts.size();

        for (auto& line_strip : glyph.strips) {
            auto s = triangle_strip_from_line_strip(line_strip);

            if (!verts.empty()) {
                // add degenerate triangle
                const auto &prev = verts.back();
                verts.push_back({prev.x, prev.y, 0});
                verts.push_back({s.front().first.x, s.front().first.y, 0});
            }

            for (const auto& v : s) {
                verts.push_back({v.first.x, v.first.y, 1});
                verts.push_back({v.second.x, v.second.y, -1});
            }
        }

        gi->num_verts = verts.size() - gi->first_vert;
        glyph_infos_[glyph.code] = std::move(gi);
    }

    geometry_->set_data(verts, {{2, GL_FLOAT, offsetof(vert, x)}, {1, GL_FLOAT, offsetof(vert, d)}});
}

std::array<GLfloat, 16> text::mvp(float x, float y) const
{
    const GLfloat a = 2.f/virt_width;
    const GLfloat b = 2.f/virt_height;

    return { a, 0, 0, a*x - 1,
             0, b, 0, b*y - 1,
             0, 0, 1, 0,
             0, 0, 0, 1 };
}

void text::draw_string(float x, float y, std::string_view str) const
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    program_->bind();
    geometry_->bind();

    for (auto ch : str) {
        if (auto& gi = glyph_infos_[ch]) {
            program_->set_uniform_matrix4("proj_modelview", mvp(x, y));
            glDrawArrays(GL_TRIANGLE_STRIP, gi->first_vert, gi->num_verts);
            x += gi->width;
        } else {
            x += 100;
        }
    }
}

int text::string_width(std::string_view str) const
{
    int width = 0;

    for (auto ch : str) {
        if (auto& gi = glyph_infos_[ch])
            width += gi->width;
        else
            width += 100;
    }

    return width;
}
