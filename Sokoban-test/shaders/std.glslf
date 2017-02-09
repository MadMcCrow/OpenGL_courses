//FRAGMENT SHADER
#version 330
layout(location = 0) out vec3 color;
in vec2 UV;
in vec3 fragmentColor;
uniform sampler2D colortexture;
void main()
{
	color = texture( colortexture, UV ).rgb;
}
