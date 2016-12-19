#ifndef _GLUTILS_H_
#define _GLUTILS_H_

#include <GL/glew.h>

GLuint compile_shader(const char * shader_source, GLenum shader_type);
GLuint load_shaders(const char * vertex_shader_source,const char * fragment_shader_source);

#endif // _GLUTILS_H_
