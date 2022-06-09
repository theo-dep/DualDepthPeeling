//--------------------------------------------------------------------------------------
// Order Independent Transparency with Dual Depth Peeling
//
// Author: Théo Devaucoup
//--------------------------------------------------------------------------------------

#version 330 core

layout(location = 0) in vec3 vertexPosition;

//out vec2 texCoord;

void main(void)
{
    gl_Position = vec4(vertexPosition, 1.);
    //texCoord = vec2(0.5*(vertexPosition.x + 1), 0.5*(vertexPosition.y + 1));
}
