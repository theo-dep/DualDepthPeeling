//--------------------------------------------------------------------------------------
// Order Independent Transparency with Dual Depth Peeling
//
// Author: Théo Devaucoup
//--------------------------------------------------------------------------------------

#version 330 core

uniform sampler2DRect TempTex;

//in vec2 texCoord;

out vec4 fragColor;

void main(void)
{
    vec2 texCoord = gl_FragCoord.xy;
    fragColor = texture(TempTex, texCoord);
    if (fragColor.a == 0.) 
    {
        discard;
    }
}
