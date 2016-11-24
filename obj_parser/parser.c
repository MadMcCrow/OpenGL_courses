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
#include "readfiles.h"
//TODO: utiliser les machines à états.

bool trychar ( char *str,const char test);
bool trynum  ( char *str,const char test);
bool tryvec  ( char *str,const char test);
bool tryface ( char *str,const char test);
bool trycom  ( char *str,const char test);

typedef struct {union { struct { int vert_index; int norm_index; int tex_index; }; int index[3]; }; bool has_uv} face;

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
			texcoord_count++;
			*(ptr++);
		}
		face_count += *(ptr++) == 'f' ? 1 : 0;
	}
	// Allocate vertices, face, etc ...
	//face* Faces = malloc(sizeof(face) * face_count);
	vec3* Norms = malloc(sizeof(vec3) * normal_count);
	vec2* TCoords = malloc(sizeof(vec2) * texcoord_count);
	vec3* Verts = malloc(sizeof(vec3) * vert_count);
	//GLfloat *vertex_buffer = calloc(18, 18 * sizeof(GLfloat)); // we want zeros in there.
	int cur   = 0;
	int cur_t = 0;
	int cur_n = 0;
	//vertex_buffer[3] = 1;
	//vertex_buffer[10] = 1;
	//vertex_buffer[17] = 1;
	ptr = str;
	printf("started reading\n");
	while (*ptr) {
		// Skip spaces
		while (isspace(*ptr)) ptr++;
		// Skip comments
		if (*ptr == '#') {
			printf("\nskipped comment");
			while (*ptr != '\n' && *ptr != 0) ptr++;
			// There might be some space after that comment, we need to skip it as well.
			continue;
		}
		if (*ptr == 'v' && isspace(ptr[1])) {
			printf("\nfound vertex");
			// We have a vertex, but it might be broken... Beware!
			ptr++;
			for (int i = 0; i < 3 && *ptr /* safety first, avoid end of string */; i++)
				Verts[cur].row[i] = strtof(ptr, &ptr); // strtof will write the position of the end of the number back into ptr!
			cur++;
		}else if ( ptr[0] == 'v' && ptr[1] == 't' && isspace(ptr[2])) {
		printf("\nfound vertex UV");
			// we have a vertex uv vector
			ptr=ptr+2;
			for (int i = 0; i < 2 && *ptr; i++)	
				TCoords[cur_t].row[i] = strtof(ptr, &ptr);
			cur_t++;
		}else if ( ptr[0] == 'v' && ptr[1] == 'n' && isspace(ptr[2])) {
		printf("\nfound vertex normal");
			// we have a vertex normal
			ptr=ptr+2;
			for (int i = 0; i < 3 && *ptr; i++)
				Norms[cur].row[i] = strtof(ptr, &ptr);
			cur_n++;
			
			
		}else if ( ptr[0] == 'f' && isspace(ptr[1]) ) {
			printf("\nfound face");
			ptr++;
			for (int i = 0; i < 3 && *ptr; i++)
			printf("%d ",strtol(ptr, (char**)NULL, 10));
			
			// let's do the faces
/*
			if (ptr[1] == '/') {
				printf("\ninvalid face\n"); // it's a broken face
				return 0;
			}else{
				printf(": valid");
				}
				// valid face

				for (int i = 0; i < 3 && *ptr; i++) {
				
				}
					
				
					int slash = 0;
					char buffer[8] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
					while ( ptr[slash] != '/') {
						slash++;
						printf("%d",slash);
						buffer[slash] = ptr[slash];
					}
					printf("buffer is : %s \n", buffer);
					int index = strtol(buffer, (char**)NULL, 10);
					if (ptr[1] == '/') {
						printf("/");
						ptr++;
					}
					printf("\nindex : %d", index);
					ptr++;
					}
					*/

				// find realloc use. how to increase the size of malloc. -> this would be easier in C++ with a dynamic table.

		}else
			// Something else. Maybe R. Fripp's personal phone number. Gotta check it out!
			ptr++;  // Nah, JK.
	}

// assertion. just to be safe.
// assert(cur == vert_count);

// we're having a non indexed approached to the problem in the main function.
// remember to free the memory;
	free(Norms);
	free(Verts);
	//free(TCoords);
	//free(vertex_buffer);
	return 0;
}

int main()
{
	char str[] = "# Blender v2.77 (sub 0) OBJ \n# www.blender.org\no Cube\nv 1.000000 -1.000000 -1.000000\nv 1.000000 -1.000000 1.000000\nv -1.000000 -1.000000 1.000000\nv -1.000000 -1.000000 -1.000000\nv 1.000000 1.000000 -0.999999\nv 0.999999 1.000000 1.000001\nv -1.000000 1.000000 1.000000\nv -1.000000 1.000000 -1.000000\nvn 0.0000 -1.0000 0.0000\nvn 0.0000 1.0000 0.0000\nvn 1.0000 0.0000 0.0000\nvn -0.0000 -0.0000 1.0000\nvn -1.0000 -0.0000 -0.0000\nvn 0.0000 0.0000 -1.0000\nf 2//1 4//1 1//1\nf 8//2 6//2 5//2\nf 5//3 2//3 1//3\nf 6//4 3//4 2//4\nf 3//5 8//5 4//5\nf 1//6 8//6 5//6\nf 2//1 3//1 4//1\nf 8//2 7//2 6//2\nf 5//3 6//3 2//3\nf 6//4 7//4 3//4\nf 3//5 7//5 8//5\nf 1//6 4//6 8//6";

	printf("\n----------------------start--------------------------\n");
	parse_obj(str);
	printf("\n-----------------------end---------------------------\n");
	return 0;
}


