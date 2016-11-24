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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifndef READFILES_H
#define READFILES_H
#include "readfiles.h"
#endif


//return the size of a file. very useful
int file_size(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == 0) return 0;
    fseek(fp, 0, SEEK_END);        //move forward until the EOF
    size_t sz = ftell(fp);
    fclose(fp);
    return sz;
#if 0
    struct stat buf;
    fstat(fp, &buf);
    int size = buf.st_size;
    return size;
#endif
}

char *read_file(char filename[])
{
	int size = file_size(filename);
        if (size < 1) return NULL;

        char *file_content = malloc(size+1);
        if (file_content == NULL) return NULL;
               
	FILE* fp = fopen(filename, "r");

	size_t n = fread(file_content, 1, size, fp);
        file_content[n] = 0;
        printf("taille predite=%d, lue=%lu\n", size, n);
	fclose(fp);

	return file_content;
}

