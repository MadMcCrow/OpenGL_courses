
#include "inputs.h"


void act_player(dir_t dir, app_t* app) { 
    if (can_move(&app->level, app->player.row, app->player.col, dir, true)) {
            move(&app->level, &app->player.row, &app->player.col, dir);
            app->player.turns++;
        } else {
            printf("cannot move\n");
        }
 
}
void reset(app_t* app) {
    lvl_load(app->level.name, &app->level);
    init_player(&app->player, &app->level); 
}
    

void key_callback(GLFWwindow* window, int key,int scancode, int action, int mods) {
    app_t* app = (app_t*)glfwGetWindowUserPointer(window);
    if (action) {
        switch(key) {
            case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, 1); break;
            default: break;
            case LEFT:  act_player(DIR_LEFT,  app);  break;
            case RIGHT: act_player(DIR_RIGHT, app);  break;
            case DOWN:  act_player(DIR_DOWN,  app);  break;
            case UP:    act_player(DIR_UP,    app);  break;
            case RESET: reset(app);                  break;
        }
    }
}
