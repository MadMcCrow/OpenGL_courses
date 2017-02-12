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

// fonction de lecture d'image
// (based on OpenGL-tutorial.org)
//-------------------------------------

GLuint load_bmp(const char * imagepath) {
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;
    // Actual RGB data
    unsigned char * data;
    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file)
    {
        printf("%s :image not found\n", imagepath);
        getchar();
        return 0;
        
    }
    // Read the header, i.e. the 54 first bytes
    // If less than 54 bytes are read, problem
    if ( fread(header, 1, 54, file)!=54 ){ 
        printf("Not a correct BMP file\n");
        return 0;
    }
    // A BMP files always begins with "BM"
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        return 0;
    }
    // Make sure this is a 24bpp file
    if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return 0;}
    if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return 0;}

    // Read the information about the image
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);
    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way
    // Create a buffer
    data = malloc(sizeof(unsigned char) *imageSize);
    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);
    // Everything is in memory now, the file wan be closed
    fclose (file);
    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    // OpenGL has now copied the data. Free our own version
    free(data);
    // ... nice trilinear filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
    glGenerateMipmap(GL_TEXTURE_2D);
return textureID;
}



// fonctions de génération de géométrie
//-------------------------------------

void gen_tile(GLuint elem_buf[2], vec3 col){
    glGenBuffers(2, elem_buf);
    GLfloat box[] =
    {    
        -0.50,  0.50, 0.00,  0.0, 0.0, 1.0,   col.r, col.g, col.b,   0,1,
        -0.50, -0.50, 0.00,  0.0, 0.0, 1.0,   col.r, col.g, col.b,   0,0,
         0.50,  0.50, 0.00,  0.0, 0.0, 1.0,   col.r, col.g, col.b,   1,1,
         0.50, -0.50, 0.00,  0.0, 0.0, 1.0,   col.r, col.g, col.b,   1,0
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
        -0.500,  0.500,  0.00,  -1.0,  0.0,  0.0,    col.r, col.g, col.b,    1,0, // 0
        -0.500, -0.500,  0.00,  -1.0,  0.0,  0.0,    col.r, col.g, col.b,    0,0, // 1
        -0.500,  0.500,  1.00,  -1.0,  0.0,  0.0,    col.r, col.g, col.b,    1,1, // 2
        -0.500, -0.500,  1.00,  -1.0,  0.0,  0.0,    col.r, col.g, col.b,    0,1, // 3
                                                           
         0.500,  0.500,  1.00,   1.0,  0.0,  0.0,    col.r, col.g, col.b,    1,1, // 4
         0.500, -0.500,  1.00,   1.0,  0.0,  0.0,    col.r, col.g, col.b,    0,1, // 5
         0.500,  0.500,  0.00,   1.0,  0.0,  0.0,    col.r, col.g, col.b,    1,0, // 6
         0.500, -0.500,  0.00,   1.0,  0.0,  0.0,    col.r, col.g, col.b,    0,0, // 7
                                                          
        -0.500, -0.500,  1.00,   0.0, -1.0,  0.0,    col.r, col.g, col.b,    0,1, // 8
        -0.500, -0.500,  0.00,   0.0, -1.0,  0.0,    col.r, col.g, col.b,    0,0, // 9
         0.500, -0.500,  1.00,   0.0, -1.0,  0.0,    col.r, col.g, col.b,    1,1, // 10
         0.500, -0.500,  0.00,   0.0, -1.0,  0.0,    col.r, col.g, col.b,    1,0, // 11
                                                       
        -0.500,  0.500,  0.00,   0.0,  1.0,  0.0,    col.r, col.g, col.b,    0,0, // 12
        -0.500,  0.500,  1.00,   0.0,  1.0,  0.0,    col.r, col.g, col.b,    0,1, // 13
         0.500,  0.500,  0.00,   0.0,  1.0,  0.0,    col.r, col.g, col.b,    1,0, // 14
         0.500,  0.500,  1.00,   0.0,  1.0,  0.0,    col.r, col.g, col.b,    1,1, // 15
                                                          
         0.500,  0.500,  0.00,   0.0,  0.0, -1.0,    col.r, col.g, col.b,    1,1, // 16
         0.500, -0.500,  0.00,   0.0,  0.0, -1.0,    col.r, col.g, col.b,    1,0, // 17
        -0.500,  0.500,  0.00,   0.0,  0.0, -1.0,    col.r, col.g, col.b,    0,1, // 18
        -0.500, -0.500,  0.00,   0.0,  0.0, -1.0,    col.r, col.g, col.b,    0,0, // 19
                                                        
        -0.500,  0.500,  1.00,   0.0,  0.0,  1.0,    col.r, col.g, col.b,    0,1, // 20
        -0.500, -0.500,  1.00,   0.0,  0.0,  1.0,    col.r, col.g, col.b,    0,0, // 21
         0.500,  0.500,  1.00,   0.0,  0.0,  1.0,    col.r, col.g, col.b,    1,1, // 22
         0.500, -0.500,  1.00,   0.0,  0.0,  1.0,    col.r, col.g, col.b,    1,0  // 23
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
    size_t vertex_size = sizeof(float) * 11;
    // activate both buffer
    glBindBuffer(GL_ARRAY_BUFFER, element_buffer[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer[1]);
    // activate the shader access
    glEnableVertexAttribArray(0); // x, y, z
    glEnableVertexAttribArray(1); // xn,yn,zn
    glEnableVertexAttribArray(2); // r, g, b
    glEnableVertexAttribArray(3); // u, v
    glVertexAttribPointer(
        0,                          // attribute 0 (cf. shader)
        3,                          // taille
        GL_FLOAT,                   // type
        GL_FALSE,                   // normalisé
        vertex_size,          // vertex size
        (void*)0                    // décalage
    );
        glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        vertex_size,
        (void*)(sizeof(float) * 3)
    );
    glVertexAttribPointer(
        2,                          
        3,                          
        GL_FLOAT,                  
        GL_FALSE,
        vertex_size,
        (void*)(sizeof(float) * 6)
    );
    glVertexAttribPointer(
        3,                          
        2,                          
        GL_FLOAT,                  
        GL_FALSE,
        vertex_size,
        (void*)(sizeof(float) * 9)
    );
    
    // Rendu de notre géométrie
    for (int i = 0; i < n; i++) {
        mat4 m = mat4_translate(array_pos[i]);
        mat4 mv = get_mv(app, m);
        mat4 mvp = get_mvp(app, m);
        glUniformMatrix4fv(app->m4mvp_id, 1, GL_FALSE, &mvp.matrix[0][0]);
        glUniformMatrix4fv(app->m4v_id, 1, GL_FALSE, &app->view.matrix[0][0]);
        glUniformMatrix4fv(app->m4m_id, 1, GL_FALSE, &m.matrix[0][0]);
        glUniformMatrix4fv(app->m4mv_id, 1, GL_FALSE, &mv.matrix[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    //close the access
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    // not necessary to unbind.
}


// fonctions principale d'affichage
// fait le lein entre les éléments de gameplay et l'affichage
//-------------------------------------

void display(GLFWwindow* window, const app_t* app, GLuint elem_buffer[5][2] , bool quad) {
    
    // 1) work on a clean basis :
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glUseProgram(app->program_id);
    
    // 1-bis) reset the M to the identity and produce a mvp

    
    static mat4 mvp;
    
    mvp = get_mvp(app, mat4_identity()); // mvp centered in 0;
    glUniformMatrix4fv(app->m4mvp_id, 1, GL_FALSE, &mvp.matrix[0][0] );
    
    // 2) render all the elements buffer avaible.
    
    int num_groundtiles = (LVL_WIDTH * LVL_HEIGHT) - ( app->level.num_boxes + app->level.num_walls);
    // draw the player
    {
        glEnable(GL_TEXTURE_2D);
        GLuint Texture_id  = glGetUniformLocation(app->program_id, "myTextureSampler");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, app->texture[3]);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(Texture_id, 0);
        vec3 a[1];
        a[0] = vec3_init(app->player.col,-1 * app->player.row,1);
        draw_elems(elem_buffer[0],1, a, app);
        
        glDisable(GL_TEXTURE_2D);
    }
    
    // draw the boxes
    {
        glEnable(GL_TEXTURE_2D);
        GLuint Texture_id  = glGetUniformLocation(app->program_id, "myTextureSampler");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, app->texture[0]);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(Texture_id, 0);
        
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
    
    glDisable(GL_TEXTURE_2D);
    }
    
    // draw the walls
    {
        glEnable(GL_TEXTURE_2D);
        GLuint Texture_id  = glGetUniformLocation(app->program_id, "myTextureSampler");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, app->texture[2]);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(Texture_id, 0);
        
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
        glDisable(GL_TEXTURE_2D);
    }
    
    // draw the ground
    {
        glEnable(GL_TEXTURE_2D);
        GLuint Texture_id  = glGetUniformLocation(app->program_id, "myTextureSampler");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, app->texture[1]);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(Texture_id, 0);
        
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
        //glDeleteTextures(1, &app->texture[0]);
        glDisable(GL_TEXTURE_2D);
    }
    
    //draw the target
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
       
    // 3) Render to a quad (aka render in a texture)
    if(quad){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,app->w,app->w);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mvp = get_mvp(app, mat4_identity()); // mvp centered in 0;
    glUniformMatrix4fv(app->m4mvp_id, 1, GL_FALSE, &mvp.matrix[0][0] );
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
    }   
    
    glfwSwapBuffers(window);
}

// fonction de setup du rendu
// from opengl-tutorial.org go look at tutorial 14 for understanding this part. slightly adapted to this program.
//-------------------------------------

bool display_setup(app_t* app) {
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
mat4 get_mvp(const app_t* app,mat4 model) {
    return mat4_product(app->viewproj, model);
}

mat4 get_mv(const app_t* app,mat4 model) {
    return mat4_product(app->view, model);
}

void set_vp(app_t* app) {
    mat4 proj     = mat4_perspective( 3.14159/2, (float)app->w / (float)app->h, .1, 100. );
    app->view     = mat4_lookAt(app->cam.loc, app->cam.look_at, app->cam.up);
    app->viewproj = mat4_product(proj, app->view);
}


//initialise the viewport
void init_mvp(app_t* app) {
    app->model     = mat4_identity();
    app->m4mvp_id  = glGetUniformLocation( app->program_id, "MVP" );
    app->m4m_id    = glGetUniformLocation( app->program_id, "M" );
    app->m4v_id    = glGetUniformLocation( app->program_id, "V" );
    app->m4mv_id   = glGetUniformLocation( app->program_id, "MV" );
    app->cam.loc       = vec3_init( 2.5,   -2.5, 2.0 );
    app->cam.look_at   = vec3_init( 12.5, -12.5, 0 );
    app->cam.up        = vec3_init( 0, 0, 1 );
    set_vp(app);
}

void init_pseudolight(app_t* app, vec3 coord){
    app->pseudolight_id = glGetUniformLocation( app->program_id, "LightPosition_world" );
    glUniform3f(app->pseudolight_id, coord.x, coord.y, coord.z);
}

// fonctions de POV - TPS
//-------------------------------------

void cam_player (app_t* app) {
    double anglex = 0;
    double angley = 0;
    glfwGetCursorPos(app->window, &anglex, &angley);
    anglex -= app->w/2;
    angley -= app->h/2;
    //glfwSetCursorPos(app->window, app->h/2, app->w/2);
    int x = 0;
    int y = 0;
     switch (app->player.forward){
        case DIR_UP:    x =  0; y =  1; break; 
        case DIR_DOWN:  x =  0; y = -1; break;
        case DIR_LEFT:  x =  1; y =  0; break;
        case DIR_RIGHT: x = -1; y =  0; break;
    }
    app->cam.loc       = vec3_init( app->player.col - 1.2 * x      ,  -1 * app->player.row  - 1.2 * y  , 5 );
    app->cam.look_at   = vec3_init( app->player.col,  -1 * app->player.row, 0.0 );
    //rotate with the mouse ;)
    app->cam.look_at = vec3_transform(mat4_translate(vec3_init( 0.01 *anglex, 0.01 * angley, 0 )),app->cam.look_at);
}





// fonctions d'affichage des FPS
//-------------------------------------
float elapsed_time(double last_call){
     // Measure speed
     double current_time = glfwGetTime();
     return 1./ (float)(current_time - last_call);
}
