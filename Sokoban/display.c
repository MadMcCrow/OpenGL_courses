// GL : glDrawArrays
// GLEW : presque tout le reste

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
#include "display.h"


void tile_corner(float* p, float* c, float x, float y, int corner, bool target) {
    // Morton Code (Z)
    int i = corner & 1;
    int j = corner >> 1;
    p[0] =  x + i;
    p[1] = -y - j;
    p[2] = 0;
    c[0] = target ? 0.5f : 1.0f;
    c[1] = target ? 0.5f : 0.5f;
    c[2] = target ? 0.5f : 0.5f;
}

void wall_face(float* p, float* c, const float* q, int face) {
    int s = face & 1;
    int u = face / 2;
    int v = (u + 1) % 3;
    int w = (u + 2) % 3;
    float ku = q[u] + (u == 1 ? -s : s);

    //negatif sur y
    int kv = v == 1 ? -1 : 1;
    int kw = w == 1 ? -1 : 1;

    p[u +  0 * 2] = ku;
    p[v +  0 * 2] = q[v];
    p[w +  0 * 2] = q[w];

    p[u +  3 * 2] = ku;
    p[v +  3 * 2] = q[v];
    p[w +  3 * 2] = q[w] + kw;

    p[u +  6 * 2] = ku;
    p[v +  6 * 2] = q[v] + kv;
    p[w +  6 * 2] = q[w];

    p[u +  9 * 2] = ku;
    p[v +  9 * 2] = q[v] + kv;
    p[w +  9 * 2] = q[w];
    
    p[u + 12 * 2] = ku;
    p[v + 12 * 2] = q[v];
    p[w + 12 * 2] = q[w] + kw;

    p[u + 15 * 2] = ku;
    p[v + 15 * 2] = q[v] + kv;
    p[w + 15 * 2] = q[w] + kw;

    for(int i = 0; i <= 15*2; i+=6){
        c[i + 0] = 0.0;   //R
        c[i + 1] = 0.3;   //G
        c[i + 2] = 0.8;   //B
    }
}


int fill_map_buffer(GLuint vert_buf, const level_t* level) {
    int num_tiles = LVL_HEIGHT * LVL_WIDTH;
    int num_walls = level->num_walls;
    int num_points = 6 * num_tiles + 5 * 6 * num_walls;
    glBindBuffer(GL_ARRAY_BUFFER, vert_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 3 * num_points, 0, GL_STATIC_DRAW);
    float* points = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    float* colors = points + 3;

    //sossol
    for (int y = 0; y < LVL_HEIGHT; y++) {
        for (int x = 0; x < LVL_WIDTH; x++) {
            float* p = points + 2 * 6 * 3 * (x + (LVL_WIDTH) * y);
            float* c = colors + 2 * 6 * 3 * (x + (LVL_WIDTH) * y);
            bool target = level->cells[x + LVL_WIDTH * y].target;
            tile_corner(p +  0 * 2, c +  0 * 2, x, y, 0, target);
            tile_corner(p +  3 * 2, c +  3 * 2, x, y, 2, target);
            tile_corner(p +  6 * 2, c +  6 * 2, x, y, 1, target);
            tile_corner(p +  9 * 2, c +  9 * 2, x, y, 1, target);
            tile_corner(p + 12 * 2, c + 12 * 2, x, y, 2, target);
            tile_corner(p + 15 * 2, c + 15 * 2, x, y, 3, target);
        }
    }

    points += LVL_HEIGHT * LVL_WIDTH * 6 * 3 * 2;
    colors += LVL_HEIGHT * LVL_WIDTH * 6 * 3 * 2;
    
    //boiboites
    #if 1
    for (int y = 0; y < LVL_HEIGHT; y++) {
        for (int x = 0; x < LVL_WIDTH; x++) {
            if (level->cells[x + LVL_WIDTH * y].wall) {
                const float p[] = {x, -y, 0};
                wall_face(points + 0 * 6 * 3 * 2, colors + 0 * 6 * 3 * 2, p, 0);
                wall_face(points + 1 * 6 * 3 * 2, colors + 1 * 6 * 3 * 2, p, 1);
                wall_face(points + 2 * 6 * 3 * 2, colors + 2 * 6 * 3 * 2, p, 2);
                wall_face(points + 3 * 6 * 3 * 2, colors + 3 * 6 * 3 * 2, p, 3);
                wall_face(points + 4 * 6 * 3 * 2, colors + 4 * 6 * 3 * 2, p, 5);
                points += 5 * 6 * 3 * 2;
                colors += 5 * 6 * 3 * 2;
            }
        }
    }
    #endif

    glUnmapBuffer(GL_ARRAY_BUFFER);
    return num_points;
}


void read_glsl(Application* app, const char* glslv, const char* glslf) {
    char* vertex_shader = NULL;
    char* fragment_shader = NULL;
    vertex_shader = read_file(glslv);
    fragment_shader = read_file(glslf);
    if (vertex_shader == NULL) exit(1);
    if (fragment_shader == NULL) exit(1);
    int shader_lenght;
    for (int ptr = 0; fragment_shader[ptr] != '\0'; ptr++) shader_lenght++;
    app->program_id = load_shaders(vertex_shader, fragment_shader);
    // pas besoin de garder les shaders en mémoire.
    free(vertex_shader);
    free(fragment_shader);
}

// fonction de rappel (regénération du contenu)
void display(GLFWwindow* window, const Application* app)
{
    glClear(GL_COLOR_BUFFER_BIT);
       
    // utilisation des shaders identifiés
    glUseProgram(app->program_id);

    glUniformMatrix4fv(app->matrix_id, 1, GL_FALSE, &app->modelviewproj.matrix[0][0] );
     
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
     // attache les données au contexte
    glBindBuffer(GL_ARRAY_BUFFER, app->vertex_buffer);
    glVertexAttribPointer(
        0,                          // attribute 0 (cf. shader)
        3,                          // taille
        GL_FLOAT,                   // type
        GL_FALSE,                   // normalisé
        sizeof(float) * 6,          // vertex size
        (void*)0                    // décalage
    );
    glVertexAttribPointer(
        1,                          // attribute 1 (cf. shader)
        3,                          // taille
        GL_FLOAT,                   // type
        GL_FALSE,                   // normalisé
        sizeof(float) * 6,          // vertex size
        (void*)(sizeof(float) * 3)  // décalage
    );
    glDrawArrays(GL_TRIANGLES, 0, app->num_points);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glfwSwapBuffers(window);
}

//set the viewport
void set_mvp(Application* app) {
    mat4 projection_matrix = mat4_perspective( 3.14159/2, (float)app->w / (float)app->h, .1, 100. );
    mat4 view_matrix       = mat4_lookAt(app->cam.loc, app->cam.look_at, app->cam.up);
    app->modelviewproj     = mat4_product(mat4_product(projection_matrix, view_matrix), app->model);
}

//initialise the viewport
void init_mvp(Application* app) {
    app->model     = mat4_identity();
    app->matrix_id = glGetUniformLocation( app->program_id, "MVP" );
    app->cam.loc       = vec3_init( 2.5,   -2.5, 0.5 );
    app->cam.look_at   = vec3_init( 12.5, -12.5, 0 );
    app->cam.up        = vec3_init( 0, 0, 1 );
    set_mvp(app);
}

void cam_player (Application* app, const player* p) {
    int x = 0;
    int y = 0;
     switch (p->forward){
        case DIR_UP:    x =  0; y =  1; break; 
        case DIR_DOWN:  x =  0; y = -1; break;
        case DIR_LEFT:  x =  1; y =  0; break;
        case DIR_RIGHT: x = -1; y =  0; break;
    }
    app->cam.loc       = vec3_init( p->row + 0.5, p->col - 0.5, 0.5 );
    app->cam.look_at   = vec3_init( p->row + 100 * x  ,  p->col - 100 * y, 0 );
}




