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

typedef struct
{
    vec3 loc;
    vec3 look_at;
    vec3 up;
}Camera;

typedef struct
{
    GLFWwindow* window;
    GLuint program_id;
    GLuint vertex_buffer;
    int num_points;
    level_t level;
    int h, w;
    // WARNING CODE UNVALIDATED BY ARSENE
    Camera cam;
    mat4 model;         // matrice de transformation
    mat4 modelviewproj;
    GLuint matrix_id;   // connexion avec le vertex shader (point de vue)
    // END OF WARNING
} Application;

// fonction de rappel (requêtes de fermeture)
void close_callback(GLFWwindow* window) {
    glfwSetWindowShouldClose(window, 1);
}

// fonction de rappel (événements clavier)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch(key) {
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, 1); break;
        default: break;
    }
}

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
#if 0
void box_face_gen(float* p, float* c, const float* pos) {
// lets make an entire cube of the same color
            box_face_corner_gen(p +  0 * 2, c +  0 * 2, x, y, 0, target);
            box_face_corner_gen(p +  3 * 2, c +  3 * 2, x, y, 2, target);
            box_face_corner_gen(p +  6 * 2, c +  6 * 2, x, y, 1, target);
            box_face_corner_gen(p +  9 * 2, c +  9 * 2, x, y, 1, target);
            box_face_corner_gen(p + 12 * 2, c + 12 * 2, x, y, 2, target);
            box_face_corner_gen(p + 15 * 2, c + 15 * 2, x, y, 3, target);
}

void box_face_corner_gen(float* p, float* c, float x, float y, float z, int corner, vec3 color) {
    // Morton Code (Z)
    int i = corner & 1;
    int j = corner >> 1;
    p[0] =  x + i;
    p[1] = -y - j;
    p[2] = 0;
    c[0] = color.r;
    c[1] = color.g;
    c[2] = color.b;
}
#endif

void box_face(float* p, float* c, const float* q, int face) {
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
                box_face(points + 0 * 6 * 3 * 2, colors + 0 * 6 * 3 * 2, p, 0);
                box_face(points + 1 * 6 * 3 * 2, colors + 1 * 6 * 3 * 2, p, 1);
                box_face(points + 2 * 6 * 3 * 2, colors + 2 * 6 * 3 * 2, p, 2);
                box_face(points + 3 * 6 * 3 * 2, colors + 3 * 6 * 3 * 2, p, 3);
                box_face(points + 4 * 6 * 3 * 2, colors + 4 * 6 * 3 * 2, p, 5);
                points += 5 * 6 * 3 * 2;
                colors += 5 * 6 * 3 * 2;
            }
        }
    }
    #endif

    glUnmapBuffer(GL_ARRAY_BUFFER);
    return num_points;
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



// ------------------------- unstable work here ----------------------

void set_mvp(Application* app) {
    
    mat4 projection_matrix = mat4_perspective( 3.14159/2, (float)app->w / (float)app->h, .1, 100. );
    mat4 view_matrix       = mat4_lookAt(app->cam.loc, app->cam.look_at, app->cam.up);
    app->modelviewproj     = mat4_product(mat4_product(projection_matrix, view_matrix), app->model);
}

void init_mvp(Application* app){
    app->model     = mat4_identity();
    app->matrix_id = glGetUniformLocation( app->program_id, "MVP" );
    app->cam.loc       = vec3_init( 2.5, -2.5, 0.5 );
    app->cam.look_at   = vec3_init( 12.5, -12.5, 0 );
    app->cam.up        = vec3_init( 0, 0, 1 );
    set_mvp(app);
}

// ---------------------end of unstable work --------------------------

// programme principal
int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("not enough arguments\n");
        return 1;
    }
    
    // 1) Connexion au système graphique natif & initialisation
    if (!glfwInit()) return 1;
    



    // 2) creation d'une fenetre
    // version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    Application app;
    app.h = 600;
    app.w = 800;

    GLFWwindow* window = glfwCreateWindow( app.w, app.h, "SUPER SOKOBAN 2016 version", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }

    glfwSetWindowCloseCallback(window, close_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return 1;
    }

    static char vertex_shader[] =
        "#version 330 core\n"
        "layout(location = 0) in vec3 vertexPosition_modelspace; \n"
        "layout(location = 1) in vec3 vertexColor;"
        "uniform mat4 MVP; \n"
        "out vec3 fragmentColor; \n"
        "void main() \n"
        "{ \n"
        "  gl_Position = MVP * vec4(vertexPosition_modelspace,1) ; \n"
        "  fragmentColor = vertexColor; \n"
        "} \n";
    char fragment_shader[] =
        "#version 330 core\n"
        "in vec3 fragmentColor; \n"
        "out vec3 color; \n"
        "void main() \n"
        "{ \n"
        "  color = fragmentColor; \n"
        "} \n";
        
    app.program_id = load_shaders(vertex_shader, fragment_shader);
    if (!app.program_id)
    {
        glfwTerminate();
        return 1;
    }
    
    if (!lvl_load(argv[1], &app.level)) {
        fprintf(stderr, "cannot load level\n");
        return 1;
    }
    GLuint vao_id;
    glGenVertexArrays(2, &vao_id);
    glBindVertexArray(vao_id);
    
    glGenBuffers(1, &app.vertex_buffer);
    app.num_points = fill_map_buffer(app.vertex_buffer, &app.level);
     
    glDisable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    init_mvp(&app);
    
    glfwSetWindowUserPointer(window, &app);
    
    while (!glfwWindowShouldClose(window))
    {
        //make the map rotate endlessly ;)
        //app.cam.loc     = vec3_transform(mat4_rotate(.1, app.cam.up), app.cam.loc);
        app.cam.look_at = vec3_transform(mat4_rotate(.001, app.cam.up), app.cam.look_at);
        set_mvp(&app);
        //
        display(window, &app);


        //inputs processing :        
        
        //wait for inputs
        //glfwWaitEvents();
        
        //wait for inputs for a certain amount of time (s):
        //glfwWaitEventsTimeout(1./25.);
        
        // see if any inputs have been done:
        glfwPollEvents();
        

    
    }

    glfwTerminate();

    // that's all folks!
    printf("that's all folks!\n");
    return 0;
}
