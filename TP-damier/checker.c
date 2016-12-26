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

void mem_issue(void){
printf("fuck this shit, I'm out !");
exit(EXIT_FAILURE);
}

int Pre_Checker(int sizeX, int sizeY){
 int VertexDim = (sizeX+ 1) * (sizeY + 1) * 3;
return VertexDim;
}
int Gen_checker(int sizeX, int sizeY,  GLfloat * vbo, GLuint * indices)
{
	int line = -1;
	int column  = 0;	
	for (int i = 0; i < (sizeX+1) * (sizeY+1) * 3; i=i+3){
		if (column%(sizeX+1) == 0){
			line++;
			column =0;
			}
		vbo[i]   =(float)column;
		vbo[i+1] =(float)line;
		vbo[i+2] = 0.0;
		column++;	
	}
	return 0;
	// indices : 
	
	
	
	#if 0
	int sizev, sizef;
	sizev = (tx + 1) * (ty + 1);

	// set up the indices :
	sizef = (tx) * (ty) * 2 * 3;
	face *f = malloc(sizef * sizeof(int));  // tx * ty * 2 (triangles) * 3 (point) * sizeof(int)
	if (f == NULL){mem_issue();};
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
	// Time to load up the vertex buffer :
	pos = 0;
	for ( int i = 0; i < sizev * 3 /* because of vec3 */; i++)
		vertex_buffer_data[i] = v[pos].row[i % 3];
	// same for indices :
	pos = 0;
	for ( int i = 0; i < sizef; i++)
		indices[i] = f[pos][i % 3];
	// Free the ressources taken by the function :
	free(v);
	free(f);
	// since everything run fine :
	return 0;
	#endif
}

