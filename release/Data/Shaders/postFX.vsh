#version 330
//Input
layout (location = 0) in vec3 position;
out vec2 uv;

void main()
{
    uv = position.xy*vec2(.5) + vec2(.5);
    gl_Position = vec4(position,1.);
}
