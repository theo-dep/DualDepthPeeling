//--------------------------------------------------------------------------------------
// Order Independent Transparency with Dual Depth Peeling
//
// Author: Théo Devaucoup
//--------------------------------------------------------------------------------------

#version 330 core

layout(location = 0) out vec2 Depth;

void main(void)
{
    Depth = vec2(-gl_FragCoord.z, gl_FragCoord.z);
}
