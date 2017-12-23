#include "arrows.h"
#include "util.h"

#include "gl_check.h"
#include "gl_shader_program.h"
#include "gl_buffer.h"

namespace {
constexpr int NUM_CURVE_POINTS = 20;
}

arrows::arrows(int width, int height)
    : effect(width, height)
    , ortho_proj_{init_ortho_projection_matrix(0, width, 0, height)}
    , program_{new gl::shader_program}
    , vbo_{new gl::buffer(GL_ARRAY_BUFFER)}
{
    init_gl_resources();
    init_arrows();
}

void arrows::init_gl_resources()
{
    gl::shader vert_shader(GL_VERTEX_SHADER);
    vert_shader.compile_source_file("assets/arrow.vert");
    program_->add_shader(vert_shader);

    gl::shader geom_shader(GL_GEOMETRY_SHADER);
    geom_shader.compile_source_file("assets/arrow.geom");
    program_->add_shader(geom_shader);

    gl::shader frag_shader(GL_FRAGMENT_SHADER);
    frag_shader.compile_source_file("assets/arrow.frag");
    program_->add_shader(frag_shader);

    program_->link();

    std::array<GLfloat, NUM_CURVE_POINTS> verts;
    for (int i = 0; i < NUM_CURVE_POINTS; ++i)
        verts[i] = static_cast<float>(i)/(NUM_CURVE_POINTS - 1);
    vbo_->set_data(verts.size()*sizeof(GLfloat), verts.data());
}

void arrows::init_arrows()
{
    constexpr int NUM_ARROWS = 120;

    const auto gen_control_point = [](float min_x, float max_x, float min_y, float max_y, float min_d, float max_d) {
        vec2f p{randf(min_x, max_x), randf(min_y, max_y)};
        vec2f d = randf(min_d, max_d)*vec2f{1.0, 0.0}.rotate(randf()*2.0*M_PI);
        float phi = randf(1.0, 3.0);
        return std::make_tuple(p, d, phi);
    };

    arrows_.resize(NUM_ARROWS);
    for (auto& arrow : arrows_) {
        std::tie(arrow.p0, arrow.d0, arrow.phi0) = gen_control_point(-20, -10, .5*height_ - 10, .5*height_ + 10, 5, 10);
        std::tie(arrow.p1, arrow.d1, arrow.phi1) = gen_control_point(40, 80, .5*height_ - 200, .5*height_ + 200, 80, 120);
        std::tie(arrow.p2, arrow.d2, arrow.phi2) = gen_control_point(200, 400, .5*height_ - 200, .5*height_ + 200, 80, 120);
    }
}

void arrows::redraw(unsigned time)
{
    GL_CHECK(glEnable(GL_BLEND));
    GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    program_->bind();
    program_->set_uniform_matrix4("proj_modelview", ortho_proj_);

    vbo_->bind();
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0)));

    const float t = static_cast<float>(time)/1000.0;

    for (const auto& arrow : arrows_) {
        vec2f p0 = arrow.p0 + arrow.d0*cosf(t*arrow.phi0);
        vec2f p1 = arrow.p1 + arrow.d1*cosf(t*arrow.phi1);
        vec2f p2 = arrow.p2 + arrow.d2*cosf(t*arrow.phi1);

        program_->set_uniform_value_f("p0", p0.x, p0.y);
        program_->set_uniform_value_f("p1", p1.x, p1.y);
        program_->set_uniform_value_f("p2", p2.x, p2.y);

        program_->set_uniform_value_f("start_thickness", 40.0);
        program_->set_uniform_value_f("end_thickness", 80.0);

        GL_CHECK(glDrawArrays(GL_LINE_STRIP, 0, NUM_CURVE_POINTS));
    }
}
