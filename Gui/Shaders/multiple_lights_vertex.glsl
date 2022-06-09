#version 330 core

layout(location = 0) in vec3 vertexPosition;

uniform mat4 ModelViewProjectionMatrix;

void ShadeVertex(void);

void main(void)
{
    gl_Position = ModelViewProjectionMatrix * vec4(vertexPosition, 1.);
    ShadeVertex();
}
