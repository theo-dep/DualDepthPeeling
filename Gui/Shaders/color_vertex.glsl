#version 330
//#extension GL_ARB_separate_shader_objects : require

layout(location = 0) in vec3 vPosition;

uniform mat4 MVPMatrix;

void main()
{
    gl_Position = MVPMatrix * vec4(vPosition, 1.);
}