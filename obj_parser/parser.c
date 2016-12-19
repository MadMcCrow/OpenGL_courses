//--------------------------------------------------------------------
// This code parse a .obj file into the VBO.
// Very nice indeed.
// /!\ this code is meant for C99 or C11 only (add -std=99 to GNU-GCC)
//--------------------------------------------------------------------
#ifndef STD_LIB
#define STD_LIB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "vla.h"
#include <ctype.h>

#ifndef OPENGL_INCLUDED
#define OPENGL_INCLUDED
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glutils.h"
#include "glmath.h"
#endif

// sOBJ objects : 
typedef struct {char name [30];
		int facecount;
		int vertcount;
		int textcount;
		int normcount;
		fVLA vertices;
		fVLA normals;
		fVLA texcoord;
		iVLA facindex; }sOBJ;

sOBJ sobj_create(sOBJ object){
		//initialise the VLAs
	object.vertices  = fvla_create(3); // 3d :x,y,z.
	object.normals   = fvla_create(3); //
	object.texcoord  = fvla_create(2); // 2d :u,v.
	object.facindex  = ivla_create(3); // 3 points.
	object.vertcount = 0;
	object.facecount = 0;
	object.textcount = 0;
	object.normcount = 0;
	//send the object.
	return object;
}


int sobj_delete(sOBJ object){
	//clean the VLAs
	fvla_delete(object.vertices);
	fvla_delete(object.normals);
	fvla_delete(object.texcoord);
	ivla_delete(object.facindex);
	return 0;
}

// export to VBO and EBO
GLfloat* vbo_create(sOBJ object, GLfloat* vbo){

	vbo = malloc(sizeof(GLfloat) * object.vertcount);
	if(vbo == NULL)
	{
	mem_issue();
	}
	vbo = fvla_export(object.vertices, vbo);
	return vbo;	
}

GLuint* ebo_create(sOBJ object, GLuint* ebo){
	ebo = malloc(sizeof(GLuint) * object.facecount);
	if(ebo == NULL)
	{
	mem_issue();
	}
	ebo = (unsigned int*)ivla_export(object.facindex, (int*)ebo);
	return ebo;
}

void buffer_cleanup(GLfloat* VBD[], GLuint* EBD[], int vbdsize, int ebdsize){
	for (int i = 0; i<vbdsize; i++)
	{
		free(VBD[i]);
	}
	for (int i = 0; i<ebdsize; i++)
	{
		//free(EBD[i]);
	}
}


// vertex array read method
fVLA read_dim (char* line, int dimension, fVLA coords)
{
	char* cur = line; // protect from modifying the original content;
	while (isspace(*cur)) cur++; //spaces
	for (int i = 0; i < dimension && *cur; i++) {
		float tmp = strtof(cur, &cur);
		coords = fvla_fill(coords, tmp);
		cur++;
	}
	return coords;
}


// face array read method
iVLA read_index (char* line, int select, iVLA index)
{
	int count = 0;
 	char* cur = line; 
 	while (isspace(*cur)) cur++;
	for (int i = 0; i < 9 && *cur; i++) {
			int tmp = strtol(cur, &cur, 10); // fun fact: it simply works.
			cur++;
			if (count++%3 == select){
			index = ivla_fill(index, tmp);
			}
		}
	return index;
}

// parser function.
int obj_parse(FILE* s, GLfloat* vbo[] , GLuint* ebo[])
{
	int objcount = 0;
	_Bool useo = 0;
	sOBJ scene[10];                  // we can't have more than 10 objects in the scene.
	scene[0] = sobj_create(scene[0]);
	char line[256];                  // a line shouldn't be bigger.
	while (fgets(line, 256, s)) {
		char* cur = line;
		while (isspace(*cur)) cur++;    // spaces.
		if (*cur == '#') continue;      // comments.
	
		switch (*cur) {
		// by order of likeliness.
		case 'v':
			if (*(cur + 1) == ' ') {
				scene[objcount].vertices = read_dim(line + 1, 3, scene[objcount].vertices); // +1 -> 'v'
				scene[objcount].vertcount++;
			}else if (*(cur + 1) == 'n'){
				scene[objcount].normals = read_dim(line + 2, 3, scene[objcount].normals);   // +2 -> 'vn'
				scene[objcount].normcount++;
			}else if (*(cur + 1) == 't'){
				scene[objcount].texcoord = read_dim(line + 2, 3, scene[objcount].texcoord); // +2 -> 'vt'
				scene[objcount].textcount++;
			}
			break;
		case 'f':
			scene[objcount].facindex = read_index(line + 1 ,0, scene[objcount].facindex);
			scene[objcount].facecount++;
			break;
		case 'o':
			if (useo == 0){
				useo = 1;
			}else {
				objcount++;
				scene[objcount] = sobj_create(scene[objcount]);
			}
			strcpy(scene[objcount].name, strtok(line+1, " "));
			printf("object : %s \n", scene[objcount].name);
			break;
		default:
			printf("ignored : %s \n", line);
			break;
		}
	}
	
	
	//cleaning.
	for (int o = 0; o <= objcount; o++){
	// -----------------------------------------------------------------------------------------
	// Bon pour une raison que j'ignore, j'ai le droit d'utiliser qu'une
	// seule des deux lignes qui suivent, sinon j'ai une "segmentation fault"
	// ce qui est très con.
	// en bref la première ligne sert à copier les coordonnées de vertex de l'objet 
	// dans un vbo prêt à être envoyé au GPU.
	// la deuxième est la destruction pure et simple de l'objet.
	// 
	// et je sais vraiment pas pourquoi ( je ne free pas deux fois, 
	// je ne modifie pas les pointeurs en cours de route, etc...)
	//
	// Le pire c'est que si on ne met que l'une des deux, ça marche.
	//
	// -----------------------------------------------------------------------------------------
	vbo[o] = vbo_create(scene[o], vbo[o]);
	//sobj_delete(scene[o]);
	}
return 0;
}

// main function for test purposes.
int main(int argc, char **argv )
{
	GLfloat* VBD[10]; GLuint* EBD[10];
	if (argc < 2) {
		printf("not enought arguments\n");
		return 0;
	}
	char* filename = argv[1];
	FILE *fp = fopen(filename, "r");
	
	obj_parse(fp, VBD, EBD );
	//buffer_cleanup(VBD, EBD, 1, 1);
	return 0;
}


