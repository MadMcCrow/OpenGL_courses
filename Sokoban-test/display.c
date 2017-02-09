// GL : glDrawArrays
// GLEW : presque tout le reste

// we use libbmpread from chazomaticus, check him out on github.

#include "display.h"
#include "obj_loader.h"


vec3* gen_array_pos(vec3* pos_array, const level_t lvl, cell_t type){
    //on génère la position du joueur
    pos_array[0].x =   lvl.player_col;//x
    pos_array[0].y = - lvl.player_row; //y
    pos_array[0].z = 0; // ground level
    return pos_array;
}


GLuint LoadTexture(const char * bitmap_file) {
    GLuint texture = 0;
    bmpread_t bitmap;

    if(!bmpread(bitmap_file, 0, &bitmap))
    {
        fprintf(stderr, "%s: error loading bitmap file\n", bitmap_file);
        exit(1);
    }

    /* At this point, bitmap.width and .height hold the pixel dimensions of the
     * file, and bitmap.rgb_data holds the raw pixel data in RGB triplets.
     */

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, bitmap.width, bitmap.height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, bitmap.rgb_data);

    bmpread_free(&bitmap);

    return texture;
}


int gen_box(GLuint elem_buf[2], vec3 col){
    glGenBuffers(2, elem_buf);
    GLfloat box[] =
    {    
        -1.00,  1.00, -1.00,     col.r, col.g, col.b,
        -1.00, -1.00, -1.00,     col.r, col.g, col.b,
        -1.00,  1.00,  1.00,     col.r, col.g, col.b,
        -1.00, -1.00,  1.00,     col.r, col.g, col.b,
         1.00,  1.00,  1.00,     col.r, col.g, col.b,
         1.00, -1.00,  1.00,     col.r, col.g, col.b,
         1.00,  1.00, -1.00,     col.r, col.g, col.b,
         1.00, -1.00, -1.00,     col.r, col.g, col.b
    };

    static const GLuint indices[] = {
        0,1,2,2,1,3,
        4,5,6,6,5,7,
        3,1,5,5,1,7,
        0,2,6,6,2,4,
        6,7,0,0,7,1,
        2,3,4,4,3,5
    };
    // Buffer d'informations de vertex
    glBindBuffer(GL_ARRAY_BUFFER, elem_buf[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);

    // Buffer d'indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elem_buf[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    return 0;
}

void draw_box(GLuint element_buffer[2], int n, vec3* array_pos) {
    glBindBuffer(GL_ARRAY_BUFFER, element_buffer[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer[1]);

    // Activation d'utilisation des tableaux
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        0,                          // attribute 0 (cf. shader)
        3,                          // taille
        GL_FLOAT,                   // type
        GL_FALSE,                   // normalisé
        sizeof(float) * 6,          // vertex size
        (void*)0                    // décalage
    );
    glVertexAttribPointer(
        1,                          // attribute 0 (cf. shader)
        3,                          // taille
        GL_FLOAT,                   // type
        GL_FALSE,                   // normalisé
        sizeof(float) * 6,          // vertex size
        (void*)(void*)(sizeof(float) * 3)  // décalage
    );
    
    // Rendu de notre géométrie
    
    for (int i = 0; i < n; i++) {
        glUniform3f(0, array_pos[n].x, array_pos[n].y, array_pos[n].z );
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
}

GLuint gen_obj(obj_t o, const char* filename){
    o = LoadObjAndConvert(filename);
    return o.vb;
}

void draw_obj(obj_t object, int n, vec3* array_pos) {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
     // attache les données au contexte
    glBindBuffer(GL_ARRAY_BUFFER, object.vb);
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
    for (int i = 0; i < n; i++) {
        glUniform3f(0, array_pos[n].x, array_pos[n].y, array_pos[n].z );
        glDrawArrays(GL_TRIANGLES, 0, object.num_points);
    }
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
}

void read_glsl(GLuint* program_id, const char* glslv, const char* glslf) {
    char* vertex_shader = NULL;
    char* fragment_shader = NULL;
    vertex_shader = read_file(glslv);
    fragment_shader = read_file(glslf);
    if (vertex_shader == NULL) exit(1);
    if (fragment_shader == NULL) exit(1);
    int shader_lenght;
    for (int ptr = 0; fragment_shader[ptr] != '\0'; ptr++) shader_lenght++;
    *program_id = load_shaders(vertex_shader, fragment_shader);
    // pas besoin de garder les shaders en mémoire.
    free(vertex_shader);
    free(fragment_shader);
}

// fonction de rappel (regénération du contenu)
void display(GLFWwindow* window, const app_t* app, GLuint* elem_buffer, obj_t o) {
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glUseProgram(app->program_id);
    glUniformMatrix4fv(app->matrix_id, 1, GL_FALSE, &app->modelviewproj.matrix[0][0] );
    vec3 arrayofpos[1];
    arrayofpos[0]=vec3_init(0,0,0);
    //draw_obj(o,1, arrayofpos);
    draw_box(elem_buffer, 1, arrayofpos);
        // rendu des buffers
    // Utilisation des données des buffers
    
    glfwSwapBuffers(window);
}

//set the viewport
void set_mvp(app_t* app) {
    mat4 projection_matrix = mat4_perspective( 3.14159/2, (float)app->w / (float)app->h, .1, 100. );
    mat4 view_matrix       = mat4_lookAt(app->cam.loc, app->cam.look_at, app->cam.up);
    app->modelviewproj     = mat4_product(mat4_product(projection_matrix, view_matrix), app->model);
}

//initialise the viewport
void init_mvp(app_t* app) {
    app->model     = mat4_identity();
    app->matrix_id = glGetUniformLocation( app->program_id, "MVP" );
    app->cam.loc       = vec3_init( 2.5,   -2.5, 2.0 );
    app->cam.look_at   = vec3_init( 12.5, -12.5, 0 );
    app->cam.up        = vec3_init( 0, 0, 1 );
    set_mvp(app);
}

void cam_player (app_t* app) {
    int x = 0;
    int y = 0;
     switch (app->player.forward){
        case DIR_UP:    x =  0; y =  1; break; 
        case DIR_DOWN:  x =  0; y = -1; break;
        case DIR_LEFT:  x =  1; y =  0; break;
        case DIR_RIGHT: x = -1; y =  0; break;
    }
    app->cam.loc       = vec3_init( app->player.row + 0.5      ,  app->player.col - 0.5     , 0.5 );
    app->cam.look_at   = vec3_init( app->player.row + 100 * x  ,  app->player.col - 100 * y , 0   );
}


float elapsed_time(double last_call){
     // Measure speed
     double current_time = glfwGetTime();
     return 1./ (float)(current_time - last_call);
}
