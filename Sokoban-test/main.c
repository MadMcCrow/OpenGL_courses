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
    
    app_t app;
    app.h = 600;
    app.w = 800;

    GLFWwindow* window = glfwCreateWindow(app.w, app.h, "Soko-Muslim-BAN ! - Trump wall ultimate version", NULL, NULL);
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
    read_glsl(&(app.program_id), "./shaders/std-pos.glslv", "./shaders/std.glslf");    

    if (!app.program_id)
    {
        glfwTerminate();
        return 1;
    }
    
    if (!lvl_load(argv[1], &app.level)) {
        fprintf(stderr, "cannot load level\n");
        return 1;
    }
    
    //GLuint vao_id;
    //glGenVertexArrays(1, &vao_id);
    //glBindVertexArray(vao_id);
   
    //glGenBuffers(1, &app.vertex_buffer);
    //app.num_points = fill_map_buffer(app.vertex_buffer, &app.level);
    
    obj_t o;
    GLuint vao;
    vao = gen_obj(o, "objects/cube.obj");
    
    
    //generating an element array for a box
    GLuint elem[2];
    gen_box(elem,  vec3_init( 1, 1, 0 ));
    
    
    //glDisable(GL_CULL_FACE);
    

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    init_mvp(&app);


    
    glfwSetWindowUserPointer(window, &app);
    

    while (!glfwWindowShouldClose(window))
    {
        double frame_time = glfwGetTime();
        //make the map rotate endlessly ;)
        
        //app.cam.look_at = vec3_transform(mat4_rotate(1, app.cam.up), app.cam.look_at);
        app.cam.look_at = vec3_init( 0, 0, 0);
        set_mvp(&app);
        
        display(window, &app, elem, o);


        //inputs processing :        
        
        //wait for inputs
        //glfwWaitEvents();
        //wait for inputs for a certain amount of time (s):
        //glfwWaitEventsTimeout(1./25.);
        // see if any inputs have been done:
        glfwPollEvents(); 
        printf("%0.f fps \n", elapsed_time(frame_time));
    }

    glfwTerminate();
    // that's all folks!
    printf("that's all folks!\n");
    return 0;
}






