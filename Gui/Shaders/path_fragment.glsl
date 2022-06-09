#version 330 core

/* \brief Fragment GLSL shader that demonstrates how perform pass through fragment shader.
 * This file is a part of shader-3dcurve example (https://github.com/vicrucann/shader-3dcurve).
 * https://vicrucann.github.io/tutorials/osg-shader-3dlines/
 *
 * \author Victoria Rudakova
 * \date January 2017
 * \copyright MIT license
*/

in VertexData{
    vec2 mTexCoord;
    vec4 mColor;
} VertexIn;

out vec4 FragColor;

void main(void)
{
    FragColor = VertexIn.mColor;
}
