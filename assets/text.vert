#version 330

uniform mat4 proj_modelview;

layout(location=0) in vec2 position;
layout(location=1) in float dist;

out float vert_dist;

void main(void)
{
    gl_Position = proj_modelview*vec4(position, 0.0, 1.0);
    vert_dist = dist;
}
