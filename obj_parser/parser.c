// Do not forget -std=c99 on old versions (< 5.0) of gcc !!
//gcc -Wall -Wextra main.c -o ./test.bin -lm
#ifndef STDLIB_INCLUDED
#define STDLIB_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#ifndef OPENGL_INCLUDED
#define OPENGL_INCLUDED
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glutils.h"
#include "glmath.h"
#endif


//typedef union { struct { int vert_index; int norm_index; int tex_index; }; int index[3]; }face;

int parse_obj(char *str)
{

	// Count vertices
	int vert_count = 0, face_count = 0, normal_count = 0, texcoord_count = 0;
	char* ptr = str;

	while (*ptr) {
		if (*ptr == '#') {
			while (*ptr != '\n' && *ptr != 0) ptr++;
			// There might be some space after that comment, we need to skip it as well.
			continue;
		}
		if (*ptr == 'v' && isspace(ptr[1])) {
			vert_count++;
			*(ptr++);
		}
		if (*ptr == 'v' && ptr[1] == 'n' && isspace(ptr[2])) {
			normal_count++;
			*(ptr++);
		}
		if (*ptr == 'v' && ptr[1] == 't' && isspace(ptr[2])) {
			normal_count++;
			*(ptr++);
		}
		face_count += *(ptr++) == 'f' ? 1 : 0;
	}
	// Allocate vertices, face, etc ...
	//face* Faces = malloc(sizeof(face) * face_count);
	vec3* Norms = malloc(sizeof(vec3) * normal_count);
	vec2* TCoords = malloc(sizeof(vec2) * texcoord_count);
	vec3* Verts = malloc(sizeof(vec3) * vert_count);
	GLfloat *vertex_buffer;
	vertex_buffer = malloc(18 * sizeof(GLfloat));
	int cur   = 0;
	int cur_t = 0;
	int cur_n = 0;
	ptr = str;
	while (*ptr) {
		// Skip spaces
		while (isspace(*ptr)) ptr++;

		// Skip comments
		if (*ptr == '#') {
			while (*ptr != '\n' && *ptr != 0) ptr++;
			// There might be some space after that comment, we need to skip it as well.
			continue;
		}
		if (*ptr == 'v' && isspace(ptr[1])) {
			// We have a vertex, but it might be broken... Beware!
			ptr++;
			for (int i = 0; i < 3 && *ptr /* safety first, avoid end of string */; i++)
				// strtof will write the position of the end of the number back into ptr!
				Verts[cur].row[i] = strtof(ptr, &ptr);
			cur++;
		}else if ( ptr[0] == 'v' && ptr[1] == 't' && isspace(ptr[2])) {
			// we have a vertex uv vector
			ptr++;
			for (int i = 0; i < 2 && *ptr /* safety first, avoid end of string */; i++)
				// strtof will write the position of the end of the number back into ptr!
				TCoords[cur_t].row[i] = strtof(ptr, &ptr);
			cur_t++;
		}else if ( ptr[0] == 'v' && ptr[1] == 'n' && isspace(ptr[2])) {
			// we have a vertex normal
			ptr++;
			for (int i = 0; i < 3 && *ptr /* safety first, avoid end of string */; i++)
				// strtof will write the position of the end of the number back into ptr!
				Norms[cur].row[i] = strtof(ptr, &ptr);
			cur_n++;
		}else if ( ptr[0] == 'f' && isspace(ptr[1]) ) {
			// let's do the faces
			if (ptr[1] = '/')
				// it's a broken face
				return 0;
			else{
			
				// find realloc use. how to increase the size of malloc. -> this would be easier in C++ with a dynamic table.
			}
		}else
			// Something else. Maybe R. Fripp's personal phone number. Gotta check it out!
			*ptr++; // Nah, JK.
	}

// assertion. just to be safe.
	assert(cur == vert_count);

// we're having a non indexed approached to the problem in the main function.
// remember to free the memory;
	free(Norms);
		free(Verts);
			free(TCoords);
			free(vertex_buffer);
	return 0;
}

int main()
{
	char str[] = "#This is a comment\nv 11.00 12.00 13.10 \nv 15.00 11.20 11.10";

	printf("NEW -----------------------------------------------------\n");
	parse_obj(str);
	return 0;
}


