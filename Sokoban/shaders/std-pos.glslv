//VERTEX SHADER
#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
uniform mat4 MVP;
uniform vec3 translate;
out vec3 fragmentColor;
void main()
{
	gl_Position = MVP * vec4(vertexPosition_modelspace + translate, 1);
	fragmentColor = vertexColor;
}