#include "arrows.h"
#include "util.h"

#include "gl_shader_program.h"
#include "gl_buffer.h"
#include "gl_vertex_array.h"
#include "gl_texture.h"

namespace {
constexpr int NUM_ARROWS = 256;
constexpr int NUM_CURVE_POINTS = 30;

constexpr float VIRT_WIDTH = 910;
constexpr float VIRT_HEIGHT = 512;
}

arrows::arrows(int width, int height)
    : effect{width, height}
    , ortho_proj_{init_ortho_projection_matrix(VIRT_WIDTH, VIRT_HEIGHT)}
    , program_{new gl::shader_program}
    , vbo_{new gl::buffer(GL_ARRAY_BUFFER)}
    , vao_{new gl::vertex_array}
    , state_texture_{new gl::texture(GL_TEXTURE_RECTANGLE, 4, NUM_ARROWS, GL_RG32F)}
{
    init_gl_resources();
    init_arrows();
}

arrows::~arrows()
{
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

    struct line {
        GLfloat t0;
        GLfloat is_shadow_0;
        GLfloat t1;
        GLfloat is_shadow_1;
    };
    std::array<line, 2*(NUM_CURVE_POINTS - 1)> verts;
    for (int i = 0; i < NUM_CURVE_POINTS - 1; ++i) {
        const float t0 = static_cast<float>(i)/(NUM_CURVE_POINTS - 1);
        const float t1 = static_cast<float>(i + 1)/(NUM_CURVE_POINTS - 1);
        verts[i] = { t0, 1.0, t1, 1.0 };
        verts[i + (NUM_CURVE_POINTS - 1)] = { t0, 0.0, t1, 0.0 };
    }
    vbo_->set_data(verts.size()*sizeof(line), verts.data());

    vao_->bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), reinterpret_cast<void*>(sizeof(GLfloat)));

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

    arrows_.resize(NUM_ARROWS);
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

        const vec2f p2 = arrow.p2 + arrow.d2*cosf(t*arrow.phi1);
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

    vao_->bind();
    glDrawArraysInstanced(GL_LINES, 0, 2*2*(NUM_CURVE_POINTS - 1), NUM_ARROWS);
}
