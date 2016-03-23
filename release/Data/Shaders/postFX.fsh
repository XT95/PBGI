#version 330
uniform sampler2D tex;
uniform vec2 resolution;

in vec2 uv;
out vec4 fragColor;


const float FXAA_SPAN_MAX = 8.0;
const float FXAA_REDUCE_MUL = 0.;

#define FxaaInt2 ivec2
#define FxaaFloat2 vec2
#define FxaaTexLod0(t, p) textureLod(t, p, 0.0)
#define FxaaTexOff(t, p, o, r) textureLodOffset(t, p, 0.0, o)
vec3 FxaaPixelShader( vec4 posPos, sampler2D tex, vec2 rcpFrame);

vec3 filmicToneMapping(vec3 color)
{
        color = max(vec3(0.), color - vec3(0.004));
        color = (color * (6.2 * color + .5)) / (color * (6.2 * color + 1.7) + 0.06);
        return color;
}


vec3 Uncharted2ToneMapping(vec3 color)
{
        float A = 0.15;
        float B = 0.50;
        float C = 0.10;
        float D = 0.20;
        float E = 0.02;
        float F = 0.30;
        float W = 11.2;
        float exposure = 2.;
        color *= exposure;
        color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
        float white = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
        color /= white;
        return color;
}

void main()
{
    vec2 invRes = vec2(1.)/resolution;
    vec4 posPos = vec4(uv,0.,0.);
    posPos.zw = uv - (invRes *.5);

    vec4 col = vec4(FxaaPixelShader(posPos,tex,invRes),1.);
    col.rgb = Uncharted2ToneMapping(col.rgb*8.);

    //Gamma correction
    col.rgb = pow(col.rgb, vec3(1./2.2) );
    col = clamp(col,0.,1.) * min(.5 + .5*pow( uv.x*uv.y*(1.-uv.x)*(1.-uv.y)*20., .25),1.);
    fragColor = col;
}










vec3 FxaaPixelShader(
  vec4 posPos,       // Output of FxaaVertexShader interpolated across screen.
  sampler2D tex,     // Input texture.
  vec2 rcpFrame) // Constant {1.0/frameWidth, 1.0/frameHeight}.
{
/*--------------------------------------------------------------------------*/
    #define FXAA_REDUCE_MIN   (1.0/128.0)
    //#define FXAA_REDUCE_MUL   (1.0/8.0)
    //#define FXAA_SPAN_MAX     8.0
/*--------------------------------------------------------------------------*/
    vec3 rgbNW = FxaaTexLod0(tex, posPos.zw).xyz;
    vec3 rgbNE = FxaaTexOff(tex, posPos.zw, FxaaInt2(1,0), rcpFrame.xy).xyz;
    vec3 rgbSW = FxaaTexOff(tex, posPos.zw, FxaaInt2(0,1), rcpFrame.xy).xyz;
    vec3 rgbSE = FxaaTexOff(tex, posPos.zw, FxaaInt2(1,1), rcpFrame.xy).xyz;
    vec3 rgbM  = FxaaTexLod0(tex, posPos.xy).xyz;
/*--------------------------------------------------------------------------*/
    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);
/*--------------------------------------------------------------------------*/
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
/*--------------------------------------------------------------------------*/
    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
/*--------------------------------------------------------------------------*/
    float dirReduce = max(
        (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
        FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(FxaaFloat2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),
          max(FxaaFloat2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
          dir * rcpDirMin)) * rcpFrame.xy;
/*--------------------------------------------------------------------------*/
    vec3 rgbA = (1.0/2.0) * (
        FxaaTexLod0(tex, posPos.xy + dir * (1.0/3.0 - 0.5)).xyz +
        FxaaTexLod0(tex, posPos.xy + dir * (2.0/3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
        FxaaTexLod0(tex, posPos.xy + dir * (0.0/3.0 - 0.5)).xyz +
        FxaaTexLod0(tex, posPos.xy + dir * (3.0/3.0 - 0.5)).xyz);
    float lumaB = dot(rgbB, luma);
    if((lumaB < lumaMin) || (lumaB > lumaMax)) return rgbA;
    return rgbB; }
