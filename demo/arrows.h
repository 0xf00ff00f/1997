#include "effect.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <array>
#include <memory>
#include <vector>

namespace gl
{
class shader_program;
class texture;
} // namespace gl

class geometry;

class arrows : public effect
{
public:
    arrows(int width, int height);
    ~arrows();

    void redraw(long time) override;

private:
    void init_gl_resources();
    void init_arrows();

    struct arrow_info
    {
        glm::vec2 p0, d0;
        glm::vec2 p1, d1;
        glm::vec2 p2, d2;
        float phi0, phi1, phi2;
    };
    std::vector<arrow_info> arrows_;

    std::array<GLfloat, 16> ortho_proj_;
    std::unique_ptr<gl::shader_program> program_;
    GLuint state_ssbo_;
    std::unique_ptr<geometry> geometry_;
};
