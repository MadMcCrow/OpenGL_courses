#include "glutils.h"

#include <stdio.h>      // printf
#include <stdlib.h>     // malloc, free

/* Used gl calls:
 * glCreateShader
 * glShaderSource
 * glCompileShader
 * glGetShaderiv
 * glGetShaderInfoLog
 * glDeleteShader
 * glCreateProgram
 * glAttachShader
 * glLinkProgram
 * glGetProgramiv
 * glGetProgramInfoLog
 * glDetachShader
 * glDeleteProgram
 */
#include <GL/glew.h>

GLuint compile_shader(const char * shader_source, GLenum shader_type)
{
	GLuint shader_id = glCreateShader(shader_type);

	glShaderSource(shader_id, 1, &shader_source, NULL);

	glCompileShader(shader_id);

	GLint result = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

	if (!result) {
		printf("error while compiling ");
		switch (shader_type) {
		case GL_VERTEX_SHADER:
			printf("vertex ");
			break;
		case GL_FRAGMENT_SHADER:
			printf("fragment ");
			break;
		}
		printf("shader:");
		int log_length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
		if ( log_length > 0 ) {
			char *log_message = malloc(log_length + 1);
			if (log_message) {
				glGetShaderInfoLog(shader_id, log_length, NULL, log_message);
				printf("\n%s\n", log_message);
				free(log_message);
			}else
				printf(" (compile log message exists, but i'm unable to display it)\n");
		}else printf(" (no information)\n");
	}
	if (!result) glDeleteShader(shader_id);

	return result ? shader_id : 0;
}

GLuint load_shaders(const char * vertex_shader_source, const char * fragment_shader_source)
{
	// Create the shaders
	GLuint vertex_shader_id = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);

	if (vertex_shader_id == 0) return 0;
	GLuint fragment_shader_id = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
	if (fragment_shader_id == 0) {
		glDeleteShader(vertex_shader_id);
		return 0;
	}

	// Link the program
	GLuint program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);
	glLinkProgram(program_id);

	// Check the program
	GLint result = GL_FALSE;
	glGetProgramiv(program_id, GL_LINK_STATUS, &result);
	int log_length;
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
	if ( log_length > 0 ) {
		char *log_message = malloc(log_length + 1);
		if (log_message) {
			glGetProgramInfoLog(program_id, log_length, NULL, log_message);
			printf("%s\n", log_message);
			free(log_message);
		}else
			printf("link log message exists, but i'm unable to display ot\n");
	}


	glDetachShader(program_id, vertex_shader_id);
	glDetachShader(program_id, fragment_shader_id);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	if (!result) glDeleteProgram(program_id);
	return result ? program_id : 0;
}
