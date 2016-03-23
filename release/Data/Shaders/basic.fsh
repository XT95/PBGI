#version 330

uniform sampler2D lightDepth;
uniform sampler2D albedo;
uniform sampler2D radiosity;

in vec3 pos;
in vec3 n;
in vec2 uv;
in vec2 uvLightmap;
in vec4 lightCoord;

out vec4 fragColor;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 lightPos;

float hash( float n )//->0:1
{
    return fract(sin(n)*3538.5453);
}


void main()
{

    vec3 col = texture2D(albedo, uv).rgb * diffuseColor;

    vec3 diff = vec3(1.) * max(dot(n,normalize(lightPos-pos)), 0.);
    float shad = 1.;
    for (int i=0;i<16;i++)
        if( texture(lightDepth, lightCoord.xy*.5+.5+(vec2(hash(float(i*30+1.)),hash(float(i*50.+3.)))*2.-1.)*0.001 ).r < lightCoord.z*.5+.5-0.002 )
            shad -= 1./16.;


    vec3 indirect = texture(radiosity, uvLightmap).rgb;

    col = col*(diff*shad+indirect*.5);


    fragColor = vec4(col, 1.);
}
