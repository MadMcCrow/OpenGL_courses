// Do not forget -std=c99 on old versions (< 5.0) of gcc !!
//gcc -Wall -Wextra main.c -o ./test.bin -lm

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef union { struct { float x, y, z; }; struct { float r, g, b; }; float row[3]; }vec3;
typedef union { struct { float x, y; }; struct { float u, v; }; float row[2]; }vec2;

typedef union {struct {int vert_index; int norm_index; int tex_index;};int index[3];}face;

int parse_obj(char *str) {

	// Count vertices
	int vert_count = 0, face_count = 0, normal_count = 0, texcoord_count = 0;
	char* ptr = str;
	while (*ptr) {
	if (*ptr == '#') {
			while (*ptr != '\n' && *ptr != 0) ptr++;
			// There might be some space after that comment, we need to skip it as well.
			continue;}
		if (*ptr == 'v' && isspace(ptr[1])){
		vert_count ++;
		*(ptr++);
		}
		if (*ptr == 'v' && ptr[1] == 'n' && isspace(ptr[2])){
		normal_count ++;
		*(ptr++);
		}
		if (*ptr == 'v' && ptr[1] == 't' && isspace(ptr[2])){
		normal_count ++;
		*(ptr++);
		}
		face_count += *(ptr++) == 'f' ? 1 : 0;
	}
	// Allocate vertices, face, etc ...
	face* Faces = malloc(sizeof(face) * face_count);
	vec3* Norms = malloc(sizeof(vec3) * normal_count);
	vec2* TCoords = malloc(sizeof(vec2) * texcoord);
	vec3* Verts = malloc(sizeof(vec3) * vert_count);
	// Read the vertices
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
		}if (*ptr == 'v' && isspace(ptr[1])) {
			// We have a vertex, but it might be broken... Beware!
			ptr++;
			for (int i = 0; i < 3 && *ptr/* safety first, avoid end of string */; i++) {
				// strtof will write the position of the end of the number back into ptr!
				Verts[cur].row[i] = strtof(ptr, &ptr);
			}
			cur++;			
		}
		else if ( ptr[0] == 'v' && ptr[1] == 't' && isspace(ptr[2])) {
		// we have a vertex uv vector
		ptr++;
			for (int i = 0; i < 2 && *ptr/* safety first, avoid end of string */; i++) {
				// strtof will write the position of the end of the number back into ptr!
				TCoords[cur_t].uv.row[i] = strtof(ptr, &ptr);
			}
			cur_t++;
		}else if ( ptr[0] == 'v' && ptr[1] == 'n' && isspace(ptr[2])) {
		// we have a vertex normal
		ptr++;
			for (int i = 0; i < 3 && *ptr/* safety first, avoid end of string */; i++) {
				// strtof will write the position of the end of the number back into ptr!
				verts[cur].normal.row[i] = strtof(ptr, &ptr);
			}
			cur_n++;
		}else if ( ptr[0] == 'f' && isspace(ptr[1]) ){
		
		}else {
		// Something else. Maybe R. Fripp's personal phone number. Gotta check it out!
			*ptr++; // Nah, JK.
		}
	}

	// Assertions: check that we did not break our own rules.
	// This is a good practice, and will not cost anything because it will only be executed in DEBUG mode.
	// Please note that this assertion is here for you, not for the final user:
	// it will make the program crash with a nice error message that you can understand.
	// It is easy to break it now... Just use this as an input string: "v v v v v v".
	// It is up to you to make your code secure and handle more degenerated cases
	// (e.g. missing coordinate "v 1.0 2.0", or too many "v 1.0 1.0 1.0 1.0", ...)
	// like this one, that might come from stupid users or hackers with bad intent.
	assert(cur == vert_count);
	//assert(cur == cur_n);
	printf("le petit vecteur des familles :\n");
	printf("%2f  \n", verts[0].position.x);
	printf("%2f  \n", verts[0].position.y);
	printf("%2f  \n", verts[0].position.z);

	// Free the vertices
	free(verts);

	// Moral of the story:
	// This code is simpler, easier to understand, and probably faster than the one with strtok.
	// Additionally, it is more robust to broken inputs (try to give "v v v v v v v" as input to main_old).
return 0;
}

int main() {
	char str[] = "#This is a comment\nv 11.00 12.00 13.10 \nv 15.00 11.20 11.10";
	printf("NEW -----------------------------------------------------\n");
	parse_obj(str);
	return 0;
}
