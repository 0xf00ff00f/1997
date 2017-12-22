#version 330

layout(lines) in;
layout(triangle_strip, max_vertices=4) out;

uniform mat4 proj_modelview;

in vertex_data {
    float t;
    vec2 dir;
    float thickness;
} vertex_in[];

out vec2 tex_coord;

void main(void)
{
    vec4 u0 = vec4(-vertex_in[0].dir.y, vertex_in[0].dir.x, 0.0, 0.0)*0.5*vertex_in[0].thickness;

    gl_Position = proj_modelview*(gl_in[0].gl_Position - u0);
    tex_coord = vec2(vertex_in[0].t, 0.0);
    EmitVertex();

    gl_Position = proj_modelview*(gl_in[0].gl_Position + u0);
    tex_coord = vec2(vertex_in[0].t, 1.0);
    EmitVertex();

    vec4 u1 = vec4(-vertex_in[1].dir.y, vertex_in[1].dir.x, 0.0, 0.0)*0.5*vertex_in[1].thickness;

    gl_Position = proj_modelview*(gl_in[1].gl_Position - u1);
    tex_coord = vec2(vertex_in[1].t, 0.0);
    EmitVertex();

    gl_Position = proj_modelview*(gl_in[1].gl_Position + u1);
    tex_coord = vec2(vertex_in[1].t, 1.0);
    EmitVertex();

    EndPrimitive();
}
