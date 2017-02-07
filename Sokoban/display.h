#ifndef DISPLAY_H_
#define DISPLAY_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "readfiles.h"
#include "Cglm/glutils.h"
#include "Cglm/glmath.h"
#include "game.h"
#include "inputs.h"


typedef struct
{
    vec3 loc;
    vec3 look_at;
    vec3 up;
}Camera;

typedef struct
{
    GLuint program_id;
    GLuint tex_id;
    GLuint time_id;
    GLuint quad_vertexbuffer;
}quadstats_t;

typedef struct
{
    GLFWwindow* window;
    GLuint program_id;
    GLuint vertex_buffer;
    GLuint framebuffer;
    int num_points;
    level_t level;
    int h, w;
    Camera cam;
    mat4 model;         // matrice de transformation
    mat4 modelviewproj;
    GLuint matrix_id;   // connexion avec le vertex shader (point de vue)
    quadstats_t quad;
    GLuint rendered_texture;
} Application;






void tile_corner(float* p, float* c, float x, float y, int corner, bool target);

void wall_face(float* p, float* c, const float* q, int face);


int fill_map_buffer(GLuint vert_buf, const level_t* level);

void read_glsl(GLuint* program_id, const char* glslv, const char* glslf);

// fonction de rappel (regénération du contenu)
void display(GLFWwindow* window, const Application* app);

//set the viewport
void set_mvp(Application* app);

//initialise the viewport
void init_mvp(Application* app);

// set the camera behind the player
void cam_player (Application* app, const player* p);

//fonction de préparation du rendu dans une texture
bool ready_tex(Application* app);

// fonction de rappel de rendu dans une texture.
void display_tex(GLFWwindow* window, const Application* app);

#endif