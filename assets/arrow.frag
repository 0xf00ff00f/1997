#version 330

in vec2 tex_coord;

out vec4 frag_color;

float in_arrow(vec2 p, float a, float b, float c, float h0, float h1)
{
    // d = (x - x1)*(y2 - y1) - (y - y1)*(x2 - x1)
    float d0 = -(p.x - a) - (p.y - h0)*(b - a);
    float d1 = -(p.x - a) - (p.y - h0)*(c - a);
    float v = smoothstep(0.0, 0.01, d0)*(smoothstep(0.0, 0.005, -d1) + (1.0 - step(b + .01, p.x))*smoothstep(0.0, 0.05, h1 - p.y));
    return clamp(v, 0.0, 1.0);
}

void main(void)
{
    vec2 p = vec2(tex_coord.x, 2.0*abs(tex_coord.y - 0.5));

    float v_outer = in_arrow(p, 0.85, 1.0, 0.9, 1.0, 0.4);
    float v_inner = in_arrow(p, 0.87, 1.0, 0.92, 0.8, 0.25);

    vec3 color_outer = vec3(0.125, 0.125, 0.125);
    vec3 color_inner = vec3(1.0, 0.0, 0.0);

    frag_color = vec4(mix(color_outer, color_inner, v_inner), v_outer);
}
