/*------------------------------------------------------------------------------------------/
 | these functions are made to help read files, as the shader or .obj files.
 | they also make the conversion toward OpenGL-esque data that can be injected into main.c
 |
 |
 | this code source has been only written by Perard-Gayot Noé.
 | there is no license per-se (So basically GPL3 -AFAIK),
 | but I would really like to be mentioned as the original writer,
 | also please notify me if my work helped you (MadMcCrow on GitHub)
 |-------------------------------------------------------------------------------------------*/
#ifndef STDLIB_INCLUDED
#define STDLIB_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#ifndef READFILES_H
#define READFILES_H
#include "readfiles.h"
#endif

// this is merely to make the code easier, some sort of alias.
FILE *open_readonly(char filename[])
{
	FILE* fp = NULL;

	fp = fopen(filename, "r+");
	if (fp != NULL)
		return fp;
	else{
		fprintf( stderr, "file cannot be opened %s", filename);
		return NULL;
	}
}

//return the size of a file. very useful
int file_size(FILE *fp)
{
	fseek(fp, 0L, SEEK_END);        //move forward until the EOF
	int sz = ftell(fp);
	rewind(fp);                     //replace the file descriptor to the beginning
	return sz;
}

char *read_file(char *file_content, char filename[])
{
	FILE* fp = open_readonly(filename);
	int size = file_size(fp);

	file_content = malloc(sizeof(char) * size);
	if (file_content == NULL) {
		fprintf(stderr, "Error allocating memory for line buffer.");
		exit(1);
	}
	unsigned int count = 0;
	char ch = getc(fp);
	while (ch != EOF) {
		file_content[count] = ch;
		count++;
		ch = getc(fp);
	}
	fclose(fp);
	return file_content;
}

