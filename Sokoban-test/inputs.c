
#include "inputs.h"


void key_callback(GLFWwindow* window, int key,int scancode, int action, int mods) {
    
    app_t* app = (app_t*)glfwGetWindowUserPointer(window);
    switch(key) {
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, 1); break;
        default: break;
        case LEFT:  act_player(DIR_LEFT,  &app->player); break;
        case RIGHT: act_player(DIR_RIGHT, &app->player); break;
        case DOWN:  act_player(DIR_DOWN,  &app->player); break;
        case UP:    act_player(DIR_UP,    &app->player); break;
    }
}

 
