#ifndef DISPLAY_H_
#define DISPLAY_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
}cam_t;

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
    player_t player;
    int h, w;
    cam_t cam;
    mat4 model;         // matrice de transformation
    mat4 viewproj;
    GLuint matrix_id;   // connexion avec le vertex shader (point de vue)
    quadstats_t quad;
    GLuint rendered_texture;
    GLuint depth_texture;
} app_t;





int gen_box(GLuint elem_buf[2], vec3 col);

void draw_box(GLuint element_buffer[2], int n, vec3* array_pos ,const app_t* app);


void read_glsl(GLuint* program_id, const char* glslv, const char* glslf);

// fonction de rappel (regénération du contenu)
void display(GLFWwindow* window, const app_t* app, GLuint elem_buffer[5][2]);

//set the viewport
mat4 get_mvp(const app_t* app,mat4 matrix);

void set_vp(app_t* app);

//initialise the viewport
void init_mvp(app_t* app);

// set the camera behind the player
void cam_player (app_t* app);

//fonction de calcul des fps.
float elapsed_time(double last_call);

#endif
