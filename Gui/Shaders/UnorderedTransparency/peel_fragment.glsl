//--------------------------------------------------------------------------------------
// Order Independent Transparency with Dual Depth Peeling
//
// Author: Théo Devaucoup
//--------------------------------------------------------------------------------------

#version 330 core

uniform sampler2DRect DepthBlenderTex;
uniform sampler2DRect FrontBlenderTex;
uniform sampler2DRect OpaqueDepthTex;
uniform sampler2DRect OpaqueColorTex;
//uniform float Width;
//uniform float Height;

layout(location = 0) out vec2 Depth;
layout(location = 1) out vec4 FrontColor;
layout(location = 2) out vec4 BackColor;

#define MAX_DEPTH 1.0

vec4 ShadeFragment();

void main(void)
{
    //vec2 texCoord = vec2(gl_FragCoord.x / Width, gl_FragCoord.y / Height);
    vec2 texCoord = gl_FragCoord.xy;
    vec2 depthBlender = texture(DepthBlenderTex, texCoord).xy;
    vec4 forwardTemp = texture(FrontBlenderTex, texCoord);

    float opaqueDepth = texture(OpaqueDepthTex, texCoord).x;
    vec4 opaqueColor = texture(OpaqueColorTex, texCoord);

    // Depths and 1.0-alphaMult always increase
    // so we can use pass-through by default with MAX blending
    Depth = depthBlender;

    // Front colors always increase (DST += SRC*ALPHA_MULT)
    // so we can use pass-through by default with MAX blending
    FrontColor = forwardTemp;

    // Because over blending makes color increase or decrease,
    // we cannot pass-through by default.
    // Each pass, only one fragment writes a color greater than 0
    BackColor = vec4(0.);

    float nearestDepth = -depthBlender.x;
    float farthestDepth = depthBlender.y;
    float alphaMultiplier = 1. - forwardTemp.w;

    // window-space depth interpolated linearly in screen space
    float fragDepth = gl_FragCoord.z;
    if (fragDepth < nearestDepth || fragDepth > farthestDepth)
    {
        // Skip this depth in the peeling algorithm
        Depth = vec2(-MAX_DEPTH);
        return;
    }

    if (fragDepth > nearestDepth && fragDepth < farthestDepth && opaqueDepth > farthestDepth)
    {
        // This fragment needs to be peeled again only when opaque object is in front
        // avoid opaque object in transparency when its between two transparent objects
        Depth = vec2(-fragDepth, opaqueDepth);
        return;
    }

    // If we made it here, this fragment is on the peeled layer from last pass
    // therefore, we need to shade it, and make sure it is not peeled any farther
    vec4 color = ShadeFragment();
    Depth = vec2(-MAX_DEPTH);

    if (fragDepth == nearestDepth)
    {
        FrontColor.xyz += color.rgb * color.a * alphaMultiplier;
        FrontColor.w = 1. - alphaMultiplier * (1. - color.a);
    }
    else 
    {
        BackColor += color;
    }

    // discard or multiply after transparent color
    if (opaqueDepth < fragDepth)
    {
        if (opaqueColor.a == 1.)
        {
            discard; // opaque object in front => disable transparent objects
        }
        else // when classic transparency is used in opaque object, do not discard transparent object, mix with them
        {
            FrontColor.xyz *= opaqueColor.rgb;
        }
    }
}
