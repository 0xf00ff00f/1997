#pragma once

#include <GL/glew.h>
#include <array>

std::array<GLfloat, 16> init_ortho_projection_matrix(int min_x, int max_x, int min_y, int max_y);

float randf();
float randf(float min, float max);
