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
	if (fichier != NULL)
		return fp;
	else{
		fprintf( stderr, "file cannot be opened %s", filename);
		return NULL;
	}
	return fp;
}

//return the size of a file. very useful
int file_size(FILE *fp)
{
	fseek(fp, 0L, SEEK_END);        //move forward until the EOF
	int sz = ftell(fp);
	rewind(fp);                     //replace the file descriptor to the beginning
	return sz;
}

void read_file(char *file_content, char filename)
{
	FILE* fp = open_readonly(filename);
	int size = file_size(fp);

	*file_content = malloc(size * sizeof(char));
	unsigned int count = 0;
	char ch = getc(fp);
	while ((ch != '\n')
	       {
		       file_content[count] = ch;
		       count++;
		       ch = getc(fp);
	       }
	       fclose(fp);
	       }


/*
   // Deprecated as it was an old approach to the problem. apparently
   // it makes more sens to have vertex and fragment shader in separated files.
   // However, if for any reason it is needed to use a per-line approach,
   // this is the way to do it.

   //simply read a line
   const char *readLine(FILE *fp)
   {
        int maximumLineLength = 128;

        if (fp == NULL) {
                fprintf(stderr, "Error: file pointer is null.");
                exit(1);
        }
        char *lineBuffer = (char*)malloc(sizeof(char) * maximumLineLength);
        if (lineBuffer == NULL) {
                fprintf(stderr, "Error allocating memory for line buffer.");
                exit(1);
        }

        char ch = getc(fp);
        unsigned int count = 0;
        while ((ch != '\n') && (ch != EOF)) {
                if (count == maximumLineLength) {
                        maximumLineLength += 128;
                        lineBuffer = realloc(lineBuffer, maximumLineLength);
                        if (lineBuffer == NULL) {
                                fprintf(stderr,"Error reallocating space for line buffer.");
                                exit(1);
                        }
                }
                lineBuffer[count] = ch;
                count++;
                ch = getc(fp);
        }
    lineBuffer[count] = '\0';
    realloc(lineBuffer, count + 1);
    return lineBuffer;
   }

   //separate vertex shader and vertex shader.
   void read_shader(char *vertex_shader, char *fragment_shader, FILE *fp)
   {
        int size = file_size(fp);
   *shaders = malloc(size * sizeof(char));
        if (strcmp(readLine(fp),"//VERTEX SHADER")){
        while () // basically look for FRAGMENT SHADER
        {
        //put this into a single array with '\n' to do the trick.
        }
        }


   }
 */
