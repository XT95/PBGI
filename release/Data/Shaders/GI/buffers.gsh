#version 330

uniform ivec2 texSize;

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec3 pos[];
in vec3 norm[];
in vec2 uvTex[];

smooth out vec3 p, n;
smooth out vec2 uv;
void main()
{
    vec2 minP = gl_in[0].gl_Position.xy, maxP = gl_in[0].gl_Position.xy;

    for(int i = 1; i < gl_in.length(); i++)
    {
        minP = min(minP, gl_in[i].gl_Position.xy);
        maxP = max(maxP, gl_in[i].gl_Position.xy);
    }

    for(int i = 0; i < gl_in.length(); i++)
    {
        vec2 position = gl_in[i].gl_Position.xy;
        ivec2 texelPos = ivec2(position*(texSize));

        vec2 halfTexelSize = 1./vec2(texSize*2);

        if(position.x == minP.x)
        {
            position.x = (float(texelPos.x))/(texSize.x);
        }
        else
        {
            position.x = (float(texelPos.x)+1.)/(texSize.x);
        }

        if(position.y == minP.y)
        {
            position.y = (float(texelPos.y))/(texSize.y);
        }
        else
        {
            position.y = (float(texelPos.y)+1.)/(texSize.y);
        }

        gl_Position = vec4(-1.+2.*position, 0., 1.);
        p = pos[i];
        n = norm[i];
        uv = uvTex[i];
        EmitVertex();
    }
    EndPrimitive();
}
