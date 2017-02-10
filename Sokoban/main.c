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
    init_player(&app.player, &app.level); 
    init_pseudolight(&app, vec3_init(-1, 1, -1));
    
    // we shouldn't need this, but somehow we do ... #ButWhy
    GLuint vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    
    
    //generating an element array for a box
    GLuint Elements[5][2];
    gen_box( Elements[0],  vec3_init( 0.8, 0.0, 0.0 )); // player
    gen_box( Elements[1],  vec3_init( 0.5, 0.7, 0.5 )); // Boxes
    gen_tile(Elements[2],  vec3_init( 0.2, 0.2, 0.2 )); // Ground
    gen_box( Elements[3],  vec3_init( 1.0, 1.0, 0.0 )); // Wall
    gen_tile(Elements[4],  vec3_init( 0.5, 0.5, 0.0 )); // Targets
    
    
    //glDisable(GL_CULL_FACE);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LEQUAL);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    init_mvp(&app);
    

    
    glfwSetWindowUserPointer(window, &app);
    
    //set up the in teture rendering.
    //ready_tex(&app);

    while (!glfwWindowShouldClose(window))
    {
        // start of a new frame:
        double frame_time = glfwGetTime();
        
        // 1) camera setup
        //    make the cam be over the player
        cam_player(&app);
                
        //    make the map rotate endlessly ;)
        //app.cam.loc = vec3_transform(mat4_rotate(0.01, app.cam.up), app.cam.loc);
        //app.cam.look_at = vec3_init( 15, -15, 2);
        
        // 2) game logic
        
        update_level(&app.player, &app.level);
        
        // check for possible victory
        if (is_winning(&app.level)) {
            printf("you won in %d turns\n", app.player.turns);
            break;
        }


        // 3 ) inputs processing :
        
        // 3 possible ways
        // wait for inputs
        //glfwWaitEvents();
        // wait for inputs for a certain amount of time (s):
        //glfwWaitEventsTimeout(1./25.);
        // see if any inputs have been done:
        glfwPollEvents(); 
        
        // 3) render setup
        
        set_vp(&app);
        
        display(window, &app, Elements);
        
        // 4) debug and fps count
        //GLenum err = glGetError();
        //if ((int)err != 0) printf("%d", (int)err);
        //printf("%0.f fps \n", elapsed_time(frame_time));
    }
  
    glfwTerminate();
    // that's all folks!
    printf("that's all folks!\n");
    return 0;
}
