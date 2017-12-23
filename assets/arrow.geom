#version 330

layout(lines) in;
layout(triangle_strip, max_vertices=4) out;

uniform mat4 proj_modelview;

in vertex_data {
    float t;
    vec2 dir;
    float thickness;
    float is_shadow;
} vertex_in[];

out vertex_data {
    vec2 tex_coord;
    float is_shadow;
} vertex_out;

void main(void)
{
    vec4 u0 = vec4(-vertex_in[0].dir.y, vertex_in[0].dir.x, 0.0, 0.0)*0.5*vertex_in[0].thickness;

    gl_Position = proj_modelview*(gl_in[0].gl_Position - u0);
    vertex_out.tex_coord = vec2(vertex_in[0].t, 0.0);
    vertex_out.is_shadow = vertex_in[0].is_shadow;
    EmitVertex();

    gl_Position = proj_modelview*(gl_in[0].gl_Position + u0);
    vertex_out.tex_coord = vec2(vertex_in[0].t, 1.0);
    vertex_out.is_shadow = vertex_in[0].is_shadow;
    EmitVertex();

    vec4 u1 = vec4(-vertex_in[1].dir.y, vertex_in[1].dir.x, 0.0, 0.0)*0.5*vertex_in[1].thickness;

    gl_Position = proj_modelview*(gl_in[1].gl_Position - u1);
    vertex_out.tex_coord = vec2(vertex_in[1].t, 0.0);
    vertex_out.is_shadow = vertex_in[1].is_shadow;
    EmitVertex();

    gl_Position = proj_modelview*(gl_in[1].gl_Position + u1);
    vertex_out.tex_coord = vec2(vertex_in[1].t, 1.0);
    vertex_out.is_shadow = vertex_in[1].is_shadow;
    EmitVertex();

    EndPrimitive();
}
