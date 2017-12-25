#include "util.h"

#include <cstdlib>

std::array<GLfloat, 16> init_ortho_projection_matrix(float width, float height)
{
    const GLfloat a = 2.f/width;
    const GLfloat b = 2.f/height;

    return { a, 0, 0, -1,
             0, b, 0, -1,
             0, 0, 1,  0,
             0, 0, 0,  1 };
}

float randf()
{
    return static_cast<float>(rand())/RAND_MAX;
}

float randf(float min, float max)
{
    return min + randf()*(max - min);
}
