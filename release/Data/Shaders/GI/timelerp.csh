#version 430

uniform sampler2D last,current;
uniform float t;
uniform layout(rgba32f) image2D destTex;


layout (local_size_x = 16, local_size_y = 16) in;


void main()
{
    ivec2 texSize = textureSize(last, 0);
    ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);

    vec2 p = vec2(storePos*2+1)/vec2(texSize*2.);
    vec4 a = texture(last, p);
    vec4 b = texture(current, p);


    imageStore(destTex, storePos, mix(a,b,vec4(smoothstep(0.,1.,t))));
    //imageStore(destTex, storePos, mix(a,b,vec4(min(t,1.))));
}
