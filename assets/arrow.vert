#version 330

uniform vec2 p0;
uniform vec2 p1;
uniform vec2 p2;

uniform float start_thickness;
uniform float end_thickness;

layout(location=0) in float t;

out vertex_data {
    float t;
    vec2 dir;
    float thickness;
} vertex_out;

out vec2 v_dir;

void main(void)
{
    vec2 p = mix(mix(p0, p1, t), mix(p1, p2, t), t);
    gl_Position = vec4(p, 0.0, 1.0);

    vertex_out.t = t;
    vertex_out.dir = normalize(2.0*mix(p1 - p0, p2 - p1, t));
    vertex_out.thickness = mix(start_thickness, end_thickness, t);
}
