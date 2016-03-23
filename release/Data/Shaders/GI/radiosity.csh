#version 430

uniform sampler2D position, direction, albedo;
uniform int lastPass;
uniform layout(rgba32f) image2D destTex;
uniform ivec2 offset;
const float PI = 3.14159265359;

layout (local_size_x = 16, local_size_y = 16) in;

struct Surfel
{
    vec4 pos;
    vec4 dir;
    vec4 col;
};

Surfel getSurfel(vec2 p)
{
    Surfel s;
    s.pos = texture(position, p);
    s.dir = texture(direction, p);
    s.col = texture(albedo, p);

    return s;
}

float occ( float cosE, float cosR, float a, float d) // Element to element occlusion
{
    return max(cosE+.0,0.) * cosR * a / ( a + PI*d*d  );
}

float radiance( float cosE, float cosR, float a, float d) // Element to element radiance transfer
{
    return a*( max(cosE,0.) * max(cosR,0.) ) / ( PI*d*d + a );
} 

void main()
{
    ivec2 texSize = textureSize(position, 0);
    ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);

    vec2 p = vec2(storePos*2+1)/vec2(texSize*2.);

    Surfel rec = getSurfel(p);

    if(rec.pos.a==0.)
    {
        //imageStore(destTex, storePos, vec4(0.));
        return;
    }

    //float ao = 0.;
    vec3 gi = imageLoad(destTex, storePos).xyz;

    if(offset == ivec2(0))
        gi = vec3(0.);
    //float ao = 0.;
    for(int x = 0; x < 32; x++)
    for(int y = 0; y < 32; y++)
    {
        int tx = x+offset.x, ty = y+offset.y;
        //Little hack to get the center of the texel
        vec2 p = vec2(float(tx*2+1)/float(texSize.x*2.), float(ty*2+1)/float(texSize.y*2.));
        Surfel em = getSurfel( p ); //Get emitter info
        if( em.pos.a == 0. )
                continue;

        vec3 v = em.pos.xyz - rec.pos.xyz; // vector from the emitter to the receiver
        float d = length(v) + 1e-16; //avoid 0 to the distance squared area
        v /= d;

        float cosE = dot( -v, em.dir.xyz );
        float cosR = dot( v, rec.dir.xyz );

        gi += radiance(cosE, cosR, 3.,d) * em.col.rgb*em.col.a/4.;
     //   ao += occ(cosE,cosR, 0.03,d);
    }
    if(lastPass==1)
        gi *= rec.col.rgb;
    vec4 col = vec4( gi , 1.);
    imageStore(destTex, storePos, col);
}
