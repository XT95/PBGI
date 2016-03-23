#version 330

uniform mat4 mat;
uniform vec2 resolution;
uniform vec3 lightPos;
in vec2 v;

out vec4 FragColor;

vec3 skyColor( in vec3 rd )
{
    vec3 sundir = normalize( lightPos );
    
    float yd = min(rd.y, 0.);
    rd.y = max(rd.y, 0.);
    
    vec3 col = vec3(0.);
    
    col += vec3(.4, .4 - exp( -rd.y*20. )*.3, .0) * exp(-rd.y*9.); // Red / Green 
    col += vec3(.3, .5, .6) * (1. - exp(-rd.y*8.) ) * exp(-rd.y*.9) ; // Blue
    
    col = mix(col*1.2, vec3(.3),  1.-exp(yd*100.)); // Fog
    
    col += vec3(1.0, .8, .55) * pow( max(dot(rd,sundir),0.), 15. ) * .3; // Sun
    col += vec3(1.0, .5, .3) * pow(max(dot(rd, sundir),0.), 300.0) *3.;
    
    return col;
}
void main()
{
    vec3 dir = vec3( normalize( mat*vec4(v.xy*vec2(resolution.x/resolution.y,1),-1.5,0.0) ) );

    FragColor = vec4(skyColor(dir)*.5,1.);
}
