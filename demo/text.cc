#include "text.h"

#include "gl_check.h"
#include "vec2.h"

#include <GL/glew.h>

#include <vector>

#include <cstdio>
#include <cassert>

namespace {

constexpr float half_thickness = 8;

using line_strip = std::vector<vec2f>;

const struct glyph_info {
    int width;
    std::vector<line_strip> strips;
} glyphs[] = {
    {
        // a
        100,
        { { { 0, 88 }, { 88, 88 }, { 88, 8 }, { 8, 8 }, { 8, 16 }, { 88, 80 } } },
    },
    {
        // b
        100,
        { { { 8, 120 }, { 8, 8 }, { 88, 8 }, { 88, 16 }, { 8, 80 } } },
    },
    {
        // c
        100,
        { { { 96, 88 }, { 8, 88 }, { 8, 8 }, { 96, 8 } } },
    },
    {
        // d
        100,
        { { { 88, 120 }, { 88, 8 }, { 8, 8 }, { 8, 16 }, { 88, 80 } } },
    },
    {
        // e
        100,
        { { { 96, 8 }, { 8, 8 }, { 8, 88 }, { 88, 88 }, { 88, 80 }, { 8, 16 } } },
    },
    {
        // f
        100,
        { { { 8, -24 }, { 8, 88 }, { 96, 88 } },
          { { 8, 8 }, { 96, 8 } } },
    },
    {
        // g
        100,
        { { { 88, 8 }, { 8, 8 }, { 8, 88 }, { 88, 88 }, { 88, -24 } } },
    },
    {
        // h
        100,
        { { { 8, 120 }, { 8, 0 } },
          { { 8, 80 }, { 88, 16 }, { 88, 0 } } },
    },
    {
        // i
        20,
        { { { 8, 0 }, { 8, 96 } },
          { { 8, 104 }, { 8, 120 } } },
    },
    {
        // j
        76,
        { { { 64, 96 }, { 64, 16 }, { 24, -16 }, { 0, -16 } },
          { { 64, 104 }, { 64, 120 } } },
    },
    {
        // k
        100,
        { { { 8, 120 }, { 8, 8 }, { 96, 8 } },
          { { 8, 56 }, { 48, 88 }, { 96, 88 } } },
    },
    {
        // l
        20,
        { { { 8, 0 }, { 8, 120 } } },
    },
    {
        // m
        100,
        { { { 8, 0 }, { 8, 88 }, { 88, 88 }, { 88, 0 } },
          { { 48, 88 }, { 48, 0 } } },
    },
    {
        // n
        100,
        { { { 8, 0 }, { 8, 88 }, { 48, 88 }, { 88, 56 }, { 88, 0 } } },
    },
    {
        // o
        100,
        { { { 8, 0 }, { 8, 88 }, { 88, 88 }, { 88, 8 }, { 8, 8 } } },
    },
    {
        // p
        100,
        { { { 8, -24 }, { 8, 88 }, { 88, 88 }, { 88, 8 }, { 8, 8 } } },
    },
    {
        // q
        100,
        { { { 88, -24 }, { 88, 88 }, { 8, 88 }, { 8, 80 }, { 88, 16 } } },
    },
    {
        // r
        100,
        { { { 8, 0 }, { 8, 88 }, { 96, 88 } } },
    },
    {
        // s
        100,
        { { { 96, 88 }, { 8, 88 }, { 8, 48 }, { 88, 48 }, { 88, 8 }, { 0, 8 } } },
    },
    {
        // t
        100,
        { { { 8, 120 }, { 8, 8 }, { 96, 8 } },
          { { 8, 88 }, { 96, 88 } } },
    },
    {
        // u
        100,
        { { { 8, 96 }, { 8, 8 }, { 88, 8 }, { 88, 96 } } }
    },
    {
        // v
        100,
        { { { 8, 96 }, { 8, 40 }, { 48, 8 }, { 88, 8 }, { 88, 96 } } },
    },
    {
        // w
        100,
        { { { 8, 96 }, { 8, 8 }, { 88, 8 }, { 88, 96 } },
          { { 48, 8 }, { 48, 96 } } },
    },
    {
        // x
        100,
        { { { 8, 96 }, { 8, 80 }, { 88, 16 }, { 88, 0 } },
          { { 88, 96 }, { 88, 80 }, { 8, 16 }, { 8, 0 } } },
    },
    {
        // y
        100,
        { { { 8, 96 }, { 8, 80 }, { 88, 16 } },
          { { 88, 96 }, { 88, -24 } } },
    },
    {
        // z
        100,
        { { { 0, 88 }, { 88, 88 }, { 88, 80 }, { 8, 16 }, { 8, 8 }, { 96, 8 } } },
    },
};

void draw_line_strip(const std::vector<vec2f>& verts)
{
    assert(verts.size() >= 2);

    glBegin(GL_TRIANGLE_STRIP);

    // first

    {
        const auto& p0 = verts[0];
        const auto& p1 = verts[1];

        const vec2f d = normalized(p1 - p0);
        const vec2f n = vec2f(-d.y, d.x);
        const vec2f u = n*half_thickness;

        glVertex2f(p0.x - u.x, p0.y - u.y);
        glVertex2f(p0.x + u.x, p0.y + u.y);
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

        glVertex2f(p1.x - u.x, p1.y - u.y);
        glVertex2f(p1.x + u.x, p1.y + u.y);
    }

    // last

    {
        const auto& p0 = verts[verts.size() - 1];
        const auto& p1 = verts[verts.size() - 2];

        const vec2f d = normalized(p0 - p1);
        const vec2f n = vec2f(-d.y, d.x);
        const vec2f u = n*half_thickness;

        glVertex2f(p0.x - u.x, p0.y - u.y);
        glVertex2f(p0.x + u.x, p0.y + u.y);
    }

    glEnd();
}

void draw_text(const char *str)
{
    GL_CHECK(glPushMatrix());

    for (const char *p = str; *p; ++p) {
        char ch = *p;
        if (ch >= 'a' && ch <= 'z') {
            const auto& g = glyphs[static_cast<int>(*p - 'a')];
            for (auto& strip : g.strips)
                draw_line_strip(strip);
            GL_CHECK(glTranslatef(g.width, 0, 0));
        } else {
            GL_CHECK(glTranslatef(100, 0, 0));
        }
    }

    GL_CHECK(glPopMatrix());
}

}

void text::redraw(unsigned)
{
    GL_CHECK(glMatrixMode(GL_PROJECTION));
    GL_CHECK(glLoadIdentity());
    GL_CHECK(glOrtho(0, width_, 0, height_, -1., 1.));

    GL_CHECK(glMatrixMode(GL_MODELVIEW));
    GL_CHECK(glLoadIdentity());

    GL_CHECK(glDisable(GL_CULL_FACE));
    GL_CHECK(glDisable(GL_BLEND));

    GL_CHECK(glColor4f(1, 1, 1, 1));

    GL_CHECK(glPushMatrix());

    GL_CHECK(glScalef(.5, .5, 1.));

    GL_CHECK(glTranslatef(32, 520, 0));
    draw_text("flare");

    GL_CHECK(glTranslatef(0, -160, 0));
    draw_text("nineteen ninety seven");

    GL_CHECK(glTranslatef(0, -160, 0));
    draw_text("greetz lorem ipsum");

    GL_CHECK(glTranslatef(0, -160, 0));
    draw_text("the quick brown fox");

    GL_CHECK(glPopMatrix());
}
