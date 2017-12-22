#include "effect.h"

#include <GL/glew.h>

#include <array>
#include <memory>

namespace gl {
class shader_program;
class buffer;
}

class arrows : public effect
{
public:
    arrows(int width, int height);

    void redraw();

private:
    std::array<GLfloat, 16> ortho_proj_;
    std::unique_ptr<gl::shader_program> program_;
    std::unique_ptr<gl::buffer> vbo_;
};
