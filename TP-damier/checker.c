#ifndef STDLIB_INCLUDED
#define STDLIB_INCLUDED
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifndef GL_INCLUDED
#define GL_INCLUDED
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glutils.h"
#include "glmath.h"
#endif
#ifndef CHECKER_INCLUDED
#define CHECKER_INCLUDED
#include "checker.h"
#endif

_Bool Gen_checker_backend(int tx, int ty, GLfloat *vertex, GLuint *indices)
{
	int sizev, sizef;

	sizev = (tx + 1) * (ty + 1);
	// generate an array of coordinates :
	vec3* v = malloc(sizev * sizeof(vec3));
	int pos = 0;
	for (int i = 0; i < tx + 1; i++) {
		for (int j = 0; j < ty + 1; j++) {
			v[pos].row[0] = (float)i;
			v[pos].row[1] = (float)j;
		}

	}
	// set up the indices :
	sizef = (tx) * (ty) * 2 * 3;
	face *f = malloc(sizef * sizeof(int));  // tx * ty * 2 (triangles) * 3 (point) * sizeof(int)
	pos = 0;
	int i = 0;
	while (i < (tx * ty) + tx) {                    // the loop will go through tx*ty cases + tx jumps.
		if ((i) % (tx + 1) == 0)
			i++;                            // jump to next line.
		                                        // triangle ABC A=A B=A+1 C=A+Tx :
		f[pos][0] = i;
		f[pos][1] = i + 1;
		f[pos][2] = i + tx + 1;
		pos++;
		// triangle BCD B=A+1 C=A+Tx D= A+Tx+1 :
		f[pos][0] = i + 1;
		f[pos][1] = i + tx + 1;
		f[pos][2] = i + tx + 2;
		pos++;
		//move on to next case :
		i++;
	}
	// we extend the vertex buffer and the indices buffer
	void *vertex_realloc = realloc(vertex, (sizeof(GLfloat) * 3 * sizev) + (sizeof(GLfloat) * 12));
	if (vertex_realloc == NULL /*better safe than sorry*/ )
		return 1;
	vertex = vertex_realloc;
	void *indices_realloc = realloc(indices, (sizeof(GLuint) * 3 * sizef) + (sizeof(GLuint) * 6));
	if (indices_realloc == NULL)
		return 1;
	indices = indices_realloc;
	// Time to load up the vertex buffer :
	pos = 0;
	for ( int i = 0; i < sizev * 3 /* because of vec3 */; i++)
		vertex[i] = v[pos].row[i % 3];
	// same for indices :
	pos = 0;
	for ( int i = 0; i < sizef; i++)
		indices[i] = f[pos][i % 3];
	// Free the ressources taken by the function :
	free(v);
	free(f);
	// since everything run fine :
	return 0;
}


int Gen_checker(int x, int y, GLfloat * vertex_buffer_data, GLuint * indices, _Bool axes)
{

	vertex_buffer_data = NULL;
	indices = NULL;
	vertex_buffer_data = calloc((size_t)12, sizeof(GLfloat));
	indices = calloc((size_t)6, sizeof(GLuint));
	// adding the axes:
	for (int index = 0; index < 6; index++)
		indices[index / 2] = index / 2;
	vertex_buffer_data[3] = 10.0;
	vertex_buffer_data[7] = 10.0;
	vertex_buffer_data[11] = 10.0;
	// generating the array :
	int gen = Gen_checker_backend(x, y, vertex_buffer_data, indices);
	return gen;
}
