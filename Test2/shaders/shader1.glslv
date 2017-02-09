//VERTEX SHADER
#version 330
/*
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
uniform mat4 MVP;
out vec3 fragmentColor;
*/
attribute vec4 ambient_occlusion; 
varying float ambient_occlusion_term;
layout(location = 0) in vec3 vertexPosition_modelspace;
uniform mat4 MVP;
out vec3 fragmentColor;

void main(void)
{
	// gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
	// fragmentColor = vertexColor;
	gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
	ambient_occlusion_term = ambient_occlusion.w;
}
