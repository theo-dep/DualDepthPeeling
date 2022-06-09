//--------------------------------------------------------------------------------------
// Order Independent Transparency Vertex Shader
//
// Author: Théo Devaucoup
//--------------------------------------------------------------------------------------

#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

out vec3 normal;
out vec3 lightDir;

void ShadeVertex()
{
    normal = normalize(NormalMatrix * vertexNormal);
    lightDir = normalize((ModelViewMatrix * vec4(vertexPosition, 1.)).xyz);
}
