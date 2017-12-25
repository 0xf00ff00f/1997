#version 330

out vec4 frag_color;

in float vert_dist;

void main(void)
{
    float alpha = smoothstep(1.0, 0.7, abs(vert_dist));
    frag_color = vec4(1.0, 1.0, 1.0, alpha);
}
