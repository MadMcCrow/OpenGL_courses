
#include "inputs.h"


void key_callback(GLFWwindow* window, int key,int scancode, int action, int mods) {
    switch(key) {
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, 1); break;
        default: break;
        case LEFT:  act_player(DIR_LEFT); break;
        case RIGHT: act_player(DIR_RIGHT); break;
        case DOWN:  act_player(DIR_DOWN); break;
        case UP:    act_player(DIR_UP); break;
    }
}
