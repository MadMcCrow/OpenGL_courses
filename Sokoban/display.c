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

// aide pour les valeurs de elem_buffer ;
// elem_buffer[5] : 0 = Player, 1 = Box, 2 = Ground, 3 = Wall, 4 = Target. 

// fonctions de génération de géométrie
//-------------------------------------

void gen_tile(GLuint elem_buf[2], vec3 col){
    glGenBuffers(2, elem_buf);
    GLfloat box[] =
    {    
        -0.50,  0.50, 0.00,  0.0, 0.0, 1.0,   col.r, col.g, col.b,
        -0.50, -0.50, 0.00,  0.0, 0.0, 1.0,   col.r, col.g, col.b,
         0.50,  0.50, 0.00,  0.0, 0.0, 1.0,   col.r, col.g, col.b,
         0.50, -0.50, 0.00,  0.0, 0.0, 1.0,   col.r, col.g, col.b
    };

    static const GLuint indices[] = {
        2,3,0,0,3,1
    };
    // Buffer d'informations de vertex
    glBindBuffer(GL_ARRAY_BUFFER, elem_buf[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);
    // Buffer d'indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elem_buf[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void gen_box(GLuint elem_buf[2], vec3 col){
    glGenBuffers(2, elem_buf);
    GLfloat box[] =
    {    
        -0.49,  0.49,  0.01,  -1.0, 0.0, 0.0,   col.r, col.g, col.b,
        -0.49, -0.49,  0.01,  -1.0, 0.0, 0.0,   col.r, col.g, col.b,
        -0.49,  0.49,  1.01,  -1.0, 0.0, 0.0,   col.r, col.g, col.b,
        -0.49, -0.49,  1.01,  -1.0, 0.0, 0.0,   col.r, col.g, col.b,
                      
         0.49,  0.49,  1.01,   1.0, 0.0, 0.0,   col.r, col.g, col.b,
         0.49, -0.49,  1.01,   1.0, 0.0, 0.0,   col.r, col.g, col.b,
         0.49,  0.49,  0.01,   1.0, 0.0, 0.0,   col.r, col.g, col.b,
         0.49, -0.49,  0.01,   1.0, 0.0, 0.0,   col.r, col.g, col.b,
                      
        -0.49, -0.49,  1.01,  0.0, -1.0, 0.0,   col.r, col.g, col.b,
        -0.49, -0.49,  0.01,  0.0, -1.0, 0.0,   col.r, col.g, col.b,
         0.49, -0.49,  1.01,  0.0, -1.0, 0.0,   col.r, col.g, col.b,
         0.49, -0.49,  0.01,  0.0, -1.0, 0.0,   col.r, col.g, col.b,
                      
        -0.49,  0.49,  0.01,  0.0, 1.0, 0.0,   col.r, col.g, col.b,
        -0.49,  0.49,  1.01,  0.0, 1.0, 0.0,   col.r, col.g, col.b,
         0.49,  0.49,  0.01,  0.0, 1.0, 0.0,   col.r, col.g, col.b,
         0.49,  0.49,  1.01,  0.0, 1.0, 0.0,   col.r, col.g, col.b,
                      
         0.49,  0.49,  0.01,  0.0, 0.0, -1.0,   col.r, col.g, col.b,
         0.49, -0.49,  0.01,  0.0, 0.0, -1.0,   col.r, col.g, col.b,
        -0.49,  0.49,  0.01,  0.0, 0.0, -1.0,   col.r, col.g, col.b,
        -0.49, -0.49,  0.01,  0.0, 0.0, -1.0,   col.r, col.g, col.b,
                      
        -0.49,  0.49,  1.01,  0.0, 0.0, 1.0,   col.r, col.g, col.b,
        -0.49, -0.49,  1.01,  0.0, 0.0, 1.0,   col.r, col.g, col.b,
         0.49, -0.49,  1.01,  0.0, 0.0, 1.0,   col.r, col.g, col.b,
         0.49,  0.49,  1.01,  0.0, 0.0, 1.0,   col.r, col.g, col.b,
        
        
    };

    static const GLuint indices[] = {
        0,1,2,2,1,3,        
        4,5,6,6,5,7,
        8,9,10,10,9,11,
        12,13,14,14,13,15,
        16,17,18,18,17,19,
        20,21,22,22,21,23
    };
    // Buffer d'informations de vertex
    glBindBuffer(GL_ARRAY_BUFFER, elem_buf[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);

    // Buffer d'indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elem_buf[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

// fonctions d'affichage d'un buffer 
// (peut "l'afficher" à plusieurs endroits) 
//-------------------------------------

void draw_elems(GLuint element_buffer[2], int n, vec3* array_pos ,const app_t* app) {
    // activate both buffer
    glBindBuffer(GL_ARRAY_BUFFER, element_buffer[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer[1]);
    // activate the shader access
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        0,                          // attribute 0 (cf. shader)
        3,                          // taille
        GL_FLOAT,                   // type
        GL_FALSE,                   // normalisé
        sizeof(float) * 9,          // vertex size
        (void*)0                    // décalage
    );
        glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(float) * 9,
        (void*)(void*)(sizeof(float) * 3)
    );
    glVertexAttribPointer(
        2,                          
        3,                          
        GL_FLOAT,                  
        GL_FALSE,
        sizeof(float) * 9,
        (void*)(void*)(sizeof(float) * 6)
    );
    
    // Rendu de notre géométrie
    for (int i = 0; i < n; i++) {
        mat4 mvp = get_mvp(app, mat4_translate(array_pos[i]));
        glUniformMatrix4fv(app->matrix_id, 1, GL_FALSE, &mvp.matrix[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    //close the access
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    // not necessary to unbind.
}


// fonctions principale d'affichage
// fait le lein entre les éléments de gameplay et l'affichage
//-------------------------------------

void display(GLFWwindow* window, const app_t* app, GLuint elem_buffer[5][2]) {
    static mat4 mvp;
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glUseProgram(app->program_id);
    
    mvp = get_mvp(app, mat4_identity()); // mvp centered in 0;
    glUniformMatrix4fv(app->matrix_id, 1, GL_FALSE, &mvp.matrix[0][0] );
    
    
    int num_groundtiles = (LVL_WIDTH * LVL_HEIGHT) - ( app->level.num_boxes + app->level.num_walls);
    // draw the player
    {
        vec3 a[1];
        a[0] = vec3_init(app->player.col,-1 * app->player.row,1);
        draw_elems(elem_buffer[0],1, a, app);
    }
    
    // draw the boxes
    {
        vec3 a[app->level.num_boxes];
        int n = 0;
        for (int row = 0; row < LVL_HEIGHT; row++) {
            for (int col = 0; col < LVL_WIDTH; col++) {
                cell_t cell = app->level.cells[LVL_WIDTH * row + col];
                if (cell.box) 
                {
                    a[n] = vec3_init(col,-1 * row, 1);
                    n++;
                }
        }
    }
    draw_elems(elem_buffer[1],app->level.num_boxes, a, app);
    }
    
    // draw the walls
    {
        vec3 a[app->level.num_walls];
        int n = 0;
        for (int row = 0; row < LVL_HEIGHT; row++) {
            for (int col = 0; col < LVL_WIDTH; col++) {
                cell_t cell = app->level.cells[LVL_WIDTH * row + col];
                if (cell.wall) 
                {
                    a[n] = vec3_init(col,-1 * row, 1);
                    n++;
                }
        }
        }
        draw_elems(elem_buffer[3],app->level.num_walls, a, app);
    }
    
    // draw the ground
    {
        vec3 a[num_groundtiles];
        int n = 0;
        for (int row = 0; row < LVL_HEIGHT; row++) {
            for (int col = 0; col < LVL_WIDTH; col++) {
                cell_t cell = app->level.cells[LVL_WIDTH * row + col];
                if (cell.target || cell.wall){}else
                {
                    a[n] = vec3_init(col,-1 * row, 1);
                    n++;
                }
        }
        }
        draw_elems(elem_buffer[2],num_groundtiles, a, app);
    }
    
    {
        vec3 a[app->level.num_boxes];
        int n = 0;
        for (int row = 0; row < LVL_HEIGHT; row++) {
            for (int col = 0; col < LVL_WIDTH; col++) {
                cell_t cell = app->level.cells[LVL_WIDTH * row + col];
                if (cell.target) 
                {
                    a[n] = vec3_init(col,-1 * row, 1);
                    n++;
                }
        }
    }
    draw_elems(elem_buffer[4],app->level.num_boxes, a, app);
    }    

    
    glfwSwapBuffers(window);
}


// fonctions de lecture du GLSL
//-------------------------------------

void read_glsl(GLuint* program_id, const char* glslv, const char* glslf) {
    char* vertex_shader = NULL;
    char* fragment_shader = NULL;
    vertex_shader = read_file(glslv);
    fragment_shader = read_file(glslf);
    if (vertex_shader == NULL) exit(1);
    if (fragment_shader == NULL) exit(1);
    int shader_length;
    for (int ptr = 0; fragment_shader[ptr] != '\0'; ptr++) shader_length++;
    *program_id = load_shaders(vertex_shader, fragment_shader);
    // pas besoin de garder les shaders en mémoire.
    free(vertex_shader);
    free(fragment_shader);
}


// fonctions de MVP 
//-------------------------------------
mat4 get_mvp(const app_t* app,mat4 matrix) {
    return mat4_product(app->viewproj, matrix);
}

void set_vp(app_t* app) {
    mat4 projection_matrix = mat4_perspective( 3.14159/2, (float)app->w / (float)app->h, .1, 100. );
    mat4 view_matrix       = mat4_lookAt(app->cam.loc, app->cam.look_at, app->cam.up);
    app->viewproj          = mat4_product(projection_matrix, view_matrix);
}


//initialise the viewport
void init_mvp(app_t* app) {
    app->model     = mat4_identity();
    app->matrix_id = glGetUniformLocation( app->program_id, "MVP" );
    app->cam.loc       = vec3_init( 2.5,   -2.5, 2.0 );
    app->cam.look_at   = vec3_init( 12.5, -12.5, 0 );
    app->cam.up        = vec3_init( 0, 0, 1 );
    set_vp(app);
}

void init_pseudolight(app_t* app, vec3 coord){
    coord = vec3_normalize(coord);
    app->pseudolight_id = glGetUniformLocation( app->program_id, "pseudolightpos" );
    glUniform3f(app->pseudolight_id, coord.x, coord.y, coord.z);
}

// fonctions de POV - TPS
//-------------------------------------

void cam_player (app_t* app) {
    int x = 0;
    int y = 0;
     switch (app->player.forward){
        case DIR_UP:    x =  0; y =  1; break; 
        case DIR_DOWN:  x =  0; y = -1; break;
        case DIR_LEFT:  x =  1; y =  0; break;
        case DIR_RIGHT: x = -1; y =  0; break;
    }
    app->cam.loc       = vec3_init( app->player.col - 2 * x      ,  -1 * app->player.row  - 2 * y  , 10 );
    app->cam.look_at   = vec3_init( app->player.col,  -1 * app->player.row, 0.0 );
}

// fonctions d'affichage des FPS
//-------------------------------------
float elapsed_time(double last_call){
     // Measure speed
     double current_time = glfwGetTime();
     return 1./ (float)(current_time - last_call);
}
