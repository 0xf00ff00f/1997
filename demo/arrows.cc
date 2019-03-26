#include "arrows.h"

#include "util.h"
#include "geometry.h"
#include "gl_shader_program.h"
#include "gl_texture.h"

namespace
{
constexpr auto num_arrows = 256;
constexpr auto num_curve_points = 30;

constexpr auto aspect_ratio = 16.0f/9.0f;
constexpr auto virt_height = 512.f;
constexpr auto virt_width = virt_height*aspect_ratio;
}

arrows::arrows(int width, int height)
    : effect{width, height}
    , ortho_proj_{ortho_projection_matrix(virt_width, virt_height)}
    , program_{new gl::shader_program}
    , state_texture_{new gl::texture(GL_TEXTURE_RECTANGLE, 4, num_arrows, GL_RG32F)}
    , geometry_{new geometry}
{
    init_gl_resources();
    init_arrows();
}

arrows::~arrows() = default;

void arrows::init_gl_resources()
{
    program_->add_shader(GL_VERTEX_SHADER, "assets/arrow.vert");
    program_->add_shader(GL_GEOMETRY_SHADER, "assets/arrow.geom");
    program_->add_shader(GL_FRAGMENT_SHADER, "assets/arrow.frag");
    program_->link();

    struct vertex
    {
        GLfloat t;
        GLfloat is_shadow;
    };
    std::vector<vertex> verts(2*2*(num_curve_points - 1));
    for (int i = 0; i < num_curve_points - 1; ++i) {
        const float t0 = static_cast<float>(i)/(num_curve_points - 1);
        const float t1 = static_cast<float>(i + 1)/(num_curve_points - 1);
        verts[2*i] = { t0, 1.0 };
        verts[2*i + 2*(num_curve_points - 1)] = { t0, 0.0 };
        verts[2*i + 1] = { t1, 1.0 };
        verts[2*i + 2*(num_curve_points - 1) + 1] = { t1, 0.0 };
    }

    geometry_->set_data(verts, {{1, GL_FLOAT, offsetof(vertex, t)}, {1, GL_FLOAT, offsetof(vertex, is_shadow)}});

    state_texture_->allocate(GL_RG, GL_FLOAT);
    state_texture_->set_min_filter(GL_NEAREST);
    state_texture_->set_mag_filter(GL_NEAREST);
    state_texture_->set_wrap_s(GL_CLAMP_TO_EDGE);
    state_texture_->set_wrap_t(GL_CLAMP_TO_EDGE);

    state_data_.resize(state_texture_->width()*state_texture_->height()*2);
}

void arrows::init_arrows()
{
    const auto gen_control_point = [](float min_x, float max_x, float min_y, float max_y, float min_d, float max_d) {
        vec2f p{randf(min_x, max_x), randf(min_y, max_y)};
        vec2f d = randf(min_d, max_d)*vec2f{1.0, 0.0}.rotate(randf()*2.0*M_PI);
        float phi = randf(1.0, 3.0);
        return std::make_tuple(p, d, phi);
    };

    arrows_.resize(num_arrows);
    for (auto& arrow : arrows_) {
        std::tie(arrow.p0, arrow.d0, arrow.phi0) = gen_control_point(-40, -20, 256 - 10, 256 + 10, 5, 10);
        std::tie(arrow.p1, arrow.d1, arrow.phi1) = gen_control_point(40, 80, 256 - 200, 256 + 200, 80, 120);
        std::tie(arrow.p2, arrow.d2, arrow.phi2) = gen_control_point(200, 400, 256 - 200, 256 + 200, 160, 240);
    }
}

void arrows::redraw(long time)
{
    const float t = static_cast<float>(time)/1000.0;

    // XXX probably should  be computed in vertex shader
    for (size_t i = 0; i < arrows_.size(); ++i) {
        const auto& arrow = arrows_[i];
        auto data = &state_data_[i*state_texture_->width()*2];

        const vec2f p0 = arrow.p0 + arrow.d0*cosf(t*arrow.phi0);
        *data++ = p0.x;
        *data++ = p0.y;

        const vec2f p1 = arrow.p1 + arrow.d1*cosf(t*arrow.phi1);
        *data++ = p1.x;
        *data++ = p1.y;

        const vec2f p2 = arrow.p2 + arrow.d2*cosf(t*arrow.phi2);
        *data++ = p2.x;
        *data++ = p2.y;
    }

    state_texture_->set_data(GL_RG, GL_FLOAT, reinterpret_cast<const GLvoid*>(&state_data_[0]));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    program_->bind();
    program_->set_uniform_matrix4("proj_modelview", ortho_proj_);
    program_->set_uniform_f("start_thickness", 40.0);
    program_->set_uniform_f("end_thickness", 80.0);
    program_->set_uniform_f("shadow_offset", 8.0, -8.0);
    program_->set_uniform_f("resolution", width_, height_);
    program_->set_uniform_f("spotlight_center", .05, .95);
    program_->set_uniform_i("state_texture", 0); // texunit 0

    state_texture_->bind();

    geometry_->bind();
    glDrawArraysInstanced(GL_LINES, 0, 2*2*(num_curve_points - 1), num_arrows);
}
