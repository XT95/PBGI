#version 430

uniform sampler2D inp;
uniform layout(rgba32f) image2D destTex;


layout (local_size_x = 16, local_size_y = 16) in;


void main()
{
    ivec2 texSize = textureSize(inp, 0);
    ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);

    vec2 p = vec2(storePos*2+1)/vec2(texSize*2.);
    vec4 col = texture(inp, p);
    imageStore(destTex, storePos, col);
}
