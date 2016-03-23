#version 330

layout (location = 0) in vec3 Position;


out vec2 uv;

void main()
{
    uv = Position.xy*.5+.5;
    gl_Position = vec4(Position*.25*vec3(1.,16./9.,1.)-vec3(.5), 1.0);
}
