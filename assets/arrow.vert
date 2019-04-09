#version 450

uniform float start_thickness;
uniform float end_thickness;

uniform vec2 shadow_offset;

struct arrow_state
{
    vec2 p0, p1, p2;
};

layout(binding=0) buffer ArrowState
{
    arrow_state arrows[];
};

layout(location=0) in float t;
layout(location=1) in float is_shadow;

out vertex_data {
    float t;
    vec2 dir;
    float thickness;
    float is_shadow;
} vertex_out;

out vec2 v_dir;

void main(void)
{
    vec2 p0 = arrows[gl_InstanceID].p0;
    vec2 p1 = arrows[gl_InstanceID].p1;
    vec2 p2 = arrows[gl_InstanceID].p2;

    // quadratic bezier
    vec2 p = mix(mix(p0, p1, t), mix(p1, p2, t), t);
    if (is_shadow == 1.0)
        p += shadow_offset;

    gl_Position = vec4(p, 0.0, 1.0);

    vertex_out.t = t;
    vertex_out.dir = normalize(2.0*mix(p1 - p0, p2 - p1, t));
    vertex_out.thickness = mix(start_thickness, end_thickness, t);
    vertex_out.is_shadow = is_shadow;
}
