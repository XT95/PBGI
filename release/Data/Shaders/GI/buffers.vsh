#version 330
//Input
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord0;
layout (location = 3) in vec2 texcoord1;

smooth out vec3 pos, norm;
smooth out vec2 uvTex;

uniform mat4 modelMatrix;

void main()
{
    pos = (modelMatrix*vec4(position,1.)).xyz;
    norm = (transpose(inverse(modelMatrix))*vec4(normal,1.)).xyz;
    uvTex = texcoord0;
    gl_Position = vec4(texcoord1,0.,1.);
}
