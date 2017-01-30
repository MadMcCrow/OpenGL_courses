//--------------------------------------------------------------------
// This is a sokoban game
// 
//--------------------------------------------------------------------
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
#include "display.h"
#include "inputs.h"

// fonction de rappel (requêtes de fermeture)
void close_callback(GLFWwindow* window) {
    glfwSetWindowShouldClose(window, 1);
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("not enough arguments\n");
        return 1;
    }
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
    return 1;
    } 
    // 2) creation d'une fenetre
    // version 3.3
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    Application app;
    app.h = 600;
    app.w = 800;

    GLFWwindow* window = glfwCreateWindow(app.w, app.h, "Hello Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
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

    read_glsl(&app, "./shaders/shader2.glslv", "./shaders/shader2.glslf");    

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
         
    //glDisable(GL_CULL_FACE);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    init_mvp(&app);


    
    //glfwSetWindowUserPointer(window, &app);
    


    while (!glfwWindowShouldClose(window))
    {
        //make the map rotate endlessly ;)
        
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
#if 0 // old main.c maybe still useful

// main function for test purposes.
int main(int argc, char **argv )
{

	level_t level;
	bool ok = lvl_load(argv[1], &level);
	
	int player_col = level.start_col;
	int player_row = level.start_row;
	int turns = 0;
	while (true) {
        lvl_display(&level, player_row, player_col);
        if (is_winning(&level)) {
            printf("you won in %d turns\n", turns);
            break;
        }
        dir_t dir;
        while (true) {
            printf("up, down, left, right (u,d,l,r)?");
            int c = getchar();
            switch (c) {
                case 'u': dir = DIR_UP;    break;
                case 'd': dir = DIR_DOWN;  break;
                case 'l': dir = DIR_LEFT;  break;
                case 'r': dir = DIR_RIGHT; break;
                default: continue;
            }
            break;
        }

        if (can_move(&level, player_row, player_col, dir, true)) {
            move(&level, &player_row, &player_col, dir);
        } else {
            printf("cannot move\n");
        }
        turns++;
    }
	printf("\n%d", ok);
	return 0;
}
#endif





