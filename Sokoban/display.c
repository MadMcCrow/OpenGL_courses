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


int gen_box(GLuint elem_buf[2], const level_t* level, vec3 pos, vec3 col){
    glGenBuffers(2, elem_buf);
    GLfloat box[] =
    {    
        -1.00 + pos.x,  1.00 + pos.y, -1.00 + pos.z,     col.r, col.g, col.b,
        -1.00 + pos.x, -1.00 + pos.y, -1.00 + pos.z,     col.r, col.g, col.b,
        -1.00 + pos.x,  1.00 + pos.y,  1.00 + pos.z,     col.r, col.g, col.b,
        -1.00 + pos.x, -1.00 + pos.y,  1.00 + pos.z,     col.r, col.g, col.b,
         1.00 + pos.x,  1.00 + pos.y,  1.00 + pos.z,     col.r, col.g, col.b,
         1.00 + pos.x, -1.00 + pos.y,  1.00 + pos.z,     col.r, col.g, col.b,
         1.00 + pos.x,  1.00 + pos.y, -1.00 + pos.z,     col.r, col.g, col.b,
         1.00 + pos.x, -1.00 + pos.y, -1.00 + pos.z,     col.r, col.g, col.b
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

void draw_map (GLuint vertex_array, int num_points){
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
     // attache les données au contexte
    glBindBuffer(GL_ARRAY_BUFFER, vertex_array);
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
    glDrawArrays(GL_TRIANGLES, 0, num_points);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
}
void draw_box(GLuint element_buffer[2], int n) {
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
    
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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
void display(GLFWwindow* window, const Application* app, GLuint* elem_buffer)
{
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glUseProgram(app->program_id);
    glUniformMatrix4fv(app->matrix_id, 1, GL_FALSE, &app->modelviewproj.matrix[0][0] );
    draw_map(app->vertex_buffer, app->num_points);
    draw_box(elem_buffer,1);

        // rendu des buffers
    // Utilisation des données des buffers
    
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
    app->cam.loc       = vec3_init( 2.5,   -2.5, 2.0 );
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



//from opengl-tutorial.org go look at tutorial 14 for understanding this part. slightly adapted to this program.
bool ready_tex(Application* app) {
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    app->framebuffer = 0;
    glGenFramebuffers(1, &app->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, app->framebuffer);
    
    // The texture we're going to render to
    glGenTextures(1, &(app->rendered_texture));

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, app->rendered_texture);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, app->w, app->h, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    //// The depth buffer
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, app->w, app->h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
    
    
    //// Alternative : Depth texture. Slower, but you can sample it later in your shader
    /*
    glGenTextures(1, &(app->depth_texture));
    glBindTexture(GL_TEXTURE_2D,  app->depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, app->w, app->h, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    */
    
    // Set "rendered_texture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, app->rendered_texture, 0);

    //// Depth texture alternative : 
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  app->depth_texture, 1);
    
    // Ma tentative...
    //glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, app->rendered_texture, 0);
    
    //if an error occured return false.
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;
    
    // bon la en vrai, c'est tout con, 
    // on créer un carré qui fait tout l'écran sur lequel on va appliquer notre rendu.
    // c'est le principe de la feuille de papier calque posée sur l'écran.

	// The fullscreen quad's FBO
	static const GLfloat g_quad_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	};

    // on refait tout le tralala standard pour un objets et ses shaders ;)
	
	glGenBuffers(1, &(app->quad.quad_vertexbuffer));
	glBindBuffer(GL_ARRAY_BUFFER, app->quad.quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	// Create and compile our GLSL program from the shaders
    read_glsl(&(app->quad.program_id), "./shaders/Passthrough.glslv", "./shaders/Passthrough.glslf");    
	app->quad.tex_id = glGetUniformLocation(app->quad.program_id, "rendered_texture");
	app->quad.time_id = glGetUniformLocation(app->quad.program_id, "time");

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_DEPTH_ATTACHMENT};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers


    return true;
}

void display_tex(GLFWwindow* window, const Application* app,GLuint* elem_buffer)
{
    glBindFramebuffer(GL_FRAMEBUFFER, app->framebuffer);
    glViewport(0,0,app->w,app->h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
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

    
    // rendu des buffers
    // Utilisation des données des buffers
    glBindBuffer(GL_ARRAY_BUFFER, elem_buffer[0]);
    glVertexPointer( 3, GL_FLOAT, 6 * sizeof(float), 0 );
    glColorPointer( 3, GL_FLOAT, 6 * sizeof(float), ((float*)NULL + (3)) );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elem_buffer[1]);

    // Activation d'utilisation des tableaux
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );

    // Rendu de notre géométrie
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDisableClientState( GL_COLOR_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );


    // texture rendering :
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,app->w,app->w);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    glUseProgram(app->quad.program_id);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, app->rendered_texture);
    // Set our "rendered_texture" sampler to user Texture Unit 0
    glUniform1i(app->quad.tex_id, 0);

    glUniform1f(app->quad.time_id, (float)(glfwGetTime()*10.0f) );
    
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, app->quad.quad_vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
    glDisableVertexAttribArray(0);

    glfwSwapBuffers(window);
}


float elapsed_time(double last_call){
     // Measure speed
     double current_time = glfwGetTime();
     return 1./ (float)(current_time - last_call);
}
