//FRAGMENT SHADER
#version 330 core
layout(location = 0) out vec3 color;
in vec3 fragmentColor;
void main()
{
	color = fragmentColor;
}
