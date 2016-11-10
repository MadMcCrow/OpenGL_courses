// Do not forget -std=c99 on old versions (< 5.0) of gcc !!
//gcc -Wall -Wextra main.c -o ./test.bin -lm

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef union { struct { float x, y, z; }; struct { float r, g, b; }; float row[3]; }vec3;
typedef union { struct { float x, y; }; struct { float u, v; }; float row[2]; }vec2;

typedef struct {
	vec3 position;
	vec2 uv;
	vec3 normal;
}vertex;

// The original, slightly modified to prevent a SEGFAULT
int main_old()
{
//autre approche : int size = strlen("#This is a comment\nv 11.00 12.00 13.1 \n");
	int vertex_count = 0;
	// autre approche : str = malloc(sizeof(char) * size);
	char str[] = "#This is a comment\nv 11.00 12.00 13.1 \n";
	int len = strlen(str);
	char * pch;

	printf("splitting string \"%s\" into tokens : \n", str);
	//removing comments
	for (int j = 0; j < len; j++ ) {
		if ( str[j] == '#')
			// BUG 1: Only the first line is considered as a comment.
			// New code:
			while (str[j] != '\n' && str[j] != 0) str[j++] = ' ';
			/*for (int i = 0; str[i] != '\n'; i++)
				str[i] = ' ';*/
	}

    // Note:
	// strtok is an outdated function of the standard library and has several significant drawbacks:
	// It is not thread-safe (i.e not parallelizable), not secure (can cause buffer overflows), and is slow.
	// Besides, you are only interested in splitting a string into different words, which is a simpler task than what strtok does.

	//counting vertices
	printf("%s",str);
	// BUG 2: Here, pch already points to 'v'; the first line of the for loop body will skip it.
	pch = strtok(str, " \n");
	for (int j = 0; j < len; j++ ) {
		// Old code:
		//pch = strtok(NULL, " ");
		//printf("%s", pch);
		while (pch != NULL) {
			// BUG 3: strcmp returns 0 (ZERO!) when the two strings are equal.
			// Your code was counting the number of times they were different.
			// Old code:
			//if ( strcmp(pch, "v") )
			// New code:
			if (!strcmp(pch, "v"))
				vertex_count++; //just to know how many points there are in the file.
			pch = strtok(NULL, " ");
		}
	}

	// now we can create the array of struct
	vertex vertices[vertex_count];
	//now we do the splitting job.

	// BUG 4a: You cannot strtok twice, since the delimiters (i.e. the spaces)
	// have already been replaced by '\0' (end of C string) characters
	// in the original array (namely 'str').

	// The fix is non trivial:
	//    - You could replace those characters again,
	//    - You could try to only do only one strtok round (by reallocating new memory every time a new vertex is found)
	//    - You could get a rid of strtok altogether (see the note above) : that's my suggestion in the function main_new
	pch = strtok(str, " ");
	printf("%d", vertex_count);
	for (int v = 0; v < vertex_count; v++) {
		// BUG 4b: Same story as BUG 3
		// Old code:
		//if ( strcmp(pch, "v")) {
		// New code:
		if (!strcmp(pch, "v")) {
			for (int p = 0; p < 3; p++) {
				pch = strtok(NULL, " ");
				// BAD PRACTICE: strtod is for doubles. Use strtof.
				vertices[v].position.row[p] = strtod(pch, NULL);
			}
		}
	}

	// BUG 6: What's the point of this? Dissipating power to produce heat?
	// Old code:
	//while (pch != NULL) {
	//	printf("%s\n", pch);
	//	pch = strtok(NULL, " ");
	//}
	printf("le petit vecteur des familles :\n");
	printf("%2f  \n", vertices[0].position.row[0]);
	printf("%2f  \n", vertices[0].position.row[1]);
	printf("%2f  \n", vertices[0].position.row[2]);
	return 0;
}

#include <ctype.h>  // For isspace
#include <assert.h> // For assert

int parse_obj(char *str) {
	

	// Count vertices
	int vert_count = 0;
	char* ptr = str;
	while (*ptr) {
		vert_count += *(ptr++) == 'v' ? 1 : 0;
	}

	// Allocate vertices
	vertex* verts = malloc(sizeof(vertex) * vert_count);
	printf("%d vertice(s)\n", vert_count);

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
		}

		// At this point, we have a character that is not space nor a comment
		if (*ptr == 'v' && isspace(ptr[1])) {
			// We have a vertex, but it might be broken... Beware!
			ptr++;
			for (int i = 0; i < 3 && *ptr/* safety first, avoid end of string */; i++) {
				// strtof will write the position of the end of the number back into ptr!
				verts[cur].position.row[i] = strtof(ptr, &ptr);
			}
			cur++;			
		}
		else if ( ptr[0] == 'v' && ptr[1] == 't' && isspace(ptr[2])) {
		// we have a vertex uv vector
		ptr++;
			for (int i = 0; i < 2 && *ptr/* safety first, avoid end of string */; i++) {
				// strtof will write the position of the end of the number back into ptr!
				verts[cur].uv.row[i] = strtof(ptr, &ptr);
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
