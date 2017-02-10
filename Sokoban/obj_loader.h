#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#include <GL/glew.h>
#ifdef _WIN64
#define atoll(S) _atoi64(S)
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <unistd.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include <float.h>
#include <limits.h>
#include <math.h>
#include <GLFW/glfw3.h>

//most important :


typedef struct {
  GLuint vb;
  int num_triangles;
  int num_points;
} obj_t;

obj_t LoadObjAndConvert(const char* filename);

#endif
