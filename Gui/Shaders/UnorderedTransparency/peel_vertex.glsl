//--------------------------------------------------------------------------------------
// Order Independent Transparency with Dual Depth Peeling
//
// Author: Théo Devaucoup
//--------------------------------------------------------------------------------------

#version 330 core

layout(location = 0) in vec3 vertexPosition;

uniform mat4 ModelViewProjectionMatrix;

void ShadeVertex();

void main(void)
{
    gl_Position = ModelViewProjectionMatrix * vec4(vertexPosition, 1.);
    ShadeVertex();
}
