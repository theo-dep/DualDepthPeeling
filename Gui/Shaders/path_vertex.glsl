#version 330 core

/* \brief Vertex GLSL shader that demonstrates how to draw basic thick and smooth lines and bezier curves in 3D.
 * This file is a part of shader-3dcurve example (https://github.com/vicrucann/shader-3dcurve).
 * https://vicrucann.github.io/tutorials/osg-shader-3dlines/
 *
 * \author Victoria Rudakova
 * \date January 2017
 * \copyright MIT license
*/

uniform mat4 ModelViewProjectionMatrix;

layout(location = 0) in vec4 Vertex;
layout(location = 1) in vec4 Color;

out VertexData{
    vec4 mColor;
} VertexOut;

void main(void)
{
    VertexOut.mColor = Color;
    gl_Position = ModelViewProjectionMatrix * Vertex;
}
