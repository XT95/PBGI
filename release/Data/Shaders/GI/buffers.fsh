#version 330

smooth in vec3 p, n;
smooth in vec2 uv;

layout (location = 0) out vec4 outPos;
layout (location = 1) out vec4 outNorm;
layout (location = 2) out vec4 outDirectIllum;

uniform sampler2D albedo;
uniform sampler2D lightDepth;
uniform mat4 lightVPMatrix,modelMatrix;
uniform vec3 lightPos;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;

float hash( float n )//->0:1
{
    return fract(sin(n)*3538.5453);
}

void main()
{


    vec3 col = texture(albedo, uv).rgb * diffuseColor;
    float diff = max(dot(n,normalize(lightPos-p)), 0.);
    float shad = 1.;
    vec4 lightCoord = lightVPMatrix*vec4(p,1.);
    for (int i=0;i<16;i++)
        if( texture(lightDepth, lightCoord.xy*.5+.5+(vec2(hash(float(i*30+1.)),hash(float(i*50.+3.)))*2.-1.)*0.001).r < lightCoord.z*.5+.5-0.02 )
            shad -= 1./16.;



    outPos = vec4(p, 1.);
    outNorm = vec4(n, 1.);
    outDirectIllum = vec4(col, diff*shad);
}
