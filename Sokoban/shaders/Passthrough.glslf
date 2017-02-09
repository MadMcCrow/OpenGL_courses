//FRAGMENT SHADER
#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D rendered_texture;
//uniform sampler2D depth_tex;
uniform float time;

void main(){
	color = texture( rendered_texture, UV).xyz ;
}
