#version 330 core

vec4 ShadeFragment(void);

out vec4 fColor;

void main(void)
{
    fColor = ShadeFragment();
}
