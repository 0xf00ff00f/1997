#include "arrows.h"
#include "util.h"

#include "gl_check.h"
#include "gl_shader_program.h"
#include "gl_buffer.h"

namespace {
constexpr int NUM_CURVE_POINTS = 40;
}

arrows::arrows(int width, int height)
    : effect(width, height)
    , ortho_proj_{init_ortho_projection_matrix(0, width, 0, height)}
    , program_{new gl::shader_program}
    , vbo_{new gl::buffer(GL_ARRAY_BUFFER)}
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
    for (int i = 0; i < NUM_CURVE_POINTS; ++i) {
        verts[i] = static_cast<float>(i)/(NUM_CURVE_POINTS - 1);
    }
    vbo_->set_data(verts.size()*sizeof(GLfloat), verts.data());
}

void arrows::redraw()
{
    GL_CHECK(glEnable(GL_BLEND));
    GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    program_->bind();
    program_->set_uniform_matrix4("proj_modelview", ortho_proj_);

    program_->set_uniform_value_f("p0", 20.0, 40.0);
    program_->set_uniform_value_f("p1", 200.0, 320.0);
    program_->set_uniform_value_f("p2", 400.0, 20.0);

    program_->set_uniform_value_f("start_thickness", 40.0);
    program_->set_uniform_value_f("end_thickness", 80.0);

    vbo_->bind();
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0)));
    GL_CHECK(glDrawArrays(GL_LINE_STRIP, 0, NUM_CURVE_POINTS));
}
