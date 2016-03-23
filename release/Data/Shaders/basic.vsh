#version 330
//Input
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord0;
layout (location = 3) in vec2 texcoord1;

uniform sampler2D tex;
uniform mat4 modelMatrix;
uniform mat4 VPMatrix;
uniform mat4 lightVPMatrix;

smooth out vec3 pos;
smooth out vec3 n;
smooth out vec2 uv;
smooth out vec2 uvLightmap;
smooth out vec4 lightCoord;

void main()
{
    pos = (modelMatrix*vec4(position,1.)).xyz;
    n = (transpose(inverse(modelMatrix))*vec4(normal,1.)).xyz;
    uv = texcoord0;

    ivec2 texSize = textureSize(tex, 0);

    ivec2 texelPos = ivec2(texcoord1*(texSize));

    //Improve sampling ..
    int id = gl_VertexID % 4;
    uvLightmap = vec2(texelPos+.5);
    vec2 offset = vec2(.5,-.5);
    if(id == 0)
        uvLightmap += offset.xx;
    else if(id == 1)
        uvLightmap += offset.yx;
    else if(id == 2)
        uvLightmap += offset.yy;
    else if(id == 3)
        uvLightmap += offset.xy;

    uvLightmap /= vec2(texSize);
    gl_Position = VPMatrix * vec4(pos,1.);

    lightCoord = lightVPMatrix * modelMatrix * vec4(position,1.);
}
