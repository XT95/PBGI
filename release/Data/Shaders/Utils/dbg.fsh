#version 330

uniform sampler2D inp;
in vec2 uv;

out vec4 FragColor;

void main()
{
    FragColor = texture(inp,uv);
}
