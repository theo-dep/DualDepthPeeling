//--------------------------------------------------------------------------------------
// Order Independent Transparency with Dual Depth Peeling
//
// Author: Théo Devaucoup
//--------------------------------------------------------------------------------------

#version 330 core

uniform sampler2DRect OpaqueTex;
uniform sampler2DRect FrontBlenderTex;
uniform sampler2DRect BackBlenderTex;

//in vec2 texCoord;

out vec4 fragColor;

void main(void)
{
    vec2 texCoord = gl_FragCoord.xy;
    vec3 opaqueColor = texture(OpaqueTex, texCoord).rgb;
    vec4 frontColor = texture(FrontBlenderTex, texCoord);
    vec3 backColor = texture(BackBlenderTex, texCoord).rgb;

    // mix transparent and opaque objects
    float alphaMultiplier = 1. - frontColor.w;
    fragColor.rgb = frontColor.rgb + (backColor + opaqueColor) * alphaMultiplier;
}
