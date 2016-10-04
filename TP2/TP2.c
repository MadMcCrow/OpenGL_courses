#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

Glunint compile_shader(const char * shader_source, Glenum shader_type)
{
Gluint shader_id = glCreateShader(shader_type);
//previens la CG d'un nouveau shader
glShaderSource(shader_id, 1, &shader_source, NULL);
//charge coté OS  (dans le driver) le code source 
glCompileShader(shader_id);
//compile le shader
GLint result=GL_FALSE;
glGetShaderiv;
}


//loadshader
Gluint load_shaders (...)
{
//compile un vertex shader
GLuint vertex_shader_id =
	//si on échoue on abandonne
//compile un fragment shader_id
	// si echec on rejete tout
//link les 2 shaders pour faire un programme
GLuint program_id = glCreateProgram ();
//(on assemble les shaders)
glAttachShader(program_id, )
glAttachShader(program_id, ...)
//glAttachShader(program_id, ...) si il y en a plus.
glLinkProgram(program_id);
	//vérifie qu'il n'y a pas de problème
	
glDetachShader(program_id, vertex_shader_id)
}

int init_ressources(void){return 1;}
void my_close(GLFWwindow *window)
{
int *pmore =glfwGetWindowUserPointer(window);
if (pmore)
{printf("")} 
}
void goodbye (void)
{
printf("exit");
}

int main (int argc, char *argv)
{


printf("going home")
return 0;
}







//shader: 

//vertex shader-> writen in GLSL
char vertex_shader [] = 
"#version 330 core\n"
// location 0 choisi arbitrairement -> ou attribué par Glquelquechose parce que les autres numéros sont utilisé
"layout(location = 0) in vec3 vertexPosition_modelspace;" //vec3 type
"void main()"
"{"
"gl_Position.xyz = vertexPosition;"
"gl_Position.w = 1.0;"
"}";
char Fragment_shader [] = 
