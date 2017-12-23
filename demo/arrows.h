#include "effect.h"

#include "vec2.h"

#include <GL/glew.h>

#include <array>
#include <vector>
#include <memory>

namespace gl {
class shader_program;
class buffer;
}

class arrows : public effect
{
public:
    arrows(int width, int height);

    void redraw(unsigned time);

private:
    void init_gl_resources();
    void init_arrows();

    std::array<GLfloat, 16> ortho_proj_;
    std::unique_ptr<gl::shader_program> program_;
    std::unique_ptr<gl::buffer> vbo_;

    struct arrow_state {
        vec2f p0, d0;
        vec2f p1, d1;
        vec2f p2, d2;
        float phi0, phi1, phi2;
    };
    std::vector<arrow_state> arrows_;
};
