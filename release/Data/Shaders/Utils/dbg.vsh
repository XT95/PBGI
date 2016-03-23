#version 330

layout (location = 0) in vec3 Position;

uniform vec2 scale;
uniform vec2 pos;
out vec2 uv;

void main()
{
    uv = Position.xy*.5+.5;
    gl_Position = vec4((uv*scale+pos)*2.-1., 0., 1.0);
}
