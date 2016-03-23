#version 330

layout (location = 0) in vec3 Position;


out vec2 v;

void main()
{
    v = Position.xy;
    gl_Position = vec4(Position, 1.0);
}
