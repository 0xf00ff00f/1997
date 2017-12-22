#include "util.h"

std::array<GLfloat, 16> init_ortho_projection_matrix(int min_x, int max_x, int min_y, int max_y)
{
	constexpr GLfloat Z_NEAR = -1.f;
	constexpr GLfloat Z_FAR = 1.f;

	const GLfloat a = 2.f/(max_x - min_x);
	const GLfloat b = 2.f/(max_y - min_y);
	const GLfloat c = -2.f/(Z_FAR - Z_NEAR);

	const GLfloat tx = -(max_x + min_x)/(max_x - min_x);
	const GLfloat ty = -(max_y + min_y)/(max_y - min_y);
	const GLfloat tz = -(Z_FAR + Z_NEAR)/(Z_FAR - Z_NEAR);

	return { a, 0, 0, tx,
		     0, b, 0, ty,
		     0, 0, c, tz,
		     0, 0, 0, 1 };
}
