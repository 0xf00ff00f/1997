#pragma once

#include <GL/glew.h>
#include <array>

std::array<GLfloat, 16> ortho_projection_matrix(float width, float height);

float randf();
float randf(float min, float max);
