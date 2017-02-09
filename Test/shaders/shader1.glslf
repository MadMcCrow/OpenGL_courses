#version 300 es
precision highp float;
in vec3 fragmentColor;
out vec3 color;
void main()
{
color = fragmentColor;
}

