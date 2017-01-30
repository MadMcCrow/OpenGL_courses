#ifndef INPUTS_H_
#define INPUTS_H_

#include <GLFW/glfw3.h>
#include "game.h"

void key_callback(GLFWwindow* window, int key,int scancode, int action, int mods);

#define RIGHT   GLFW_KEY_RIGHT
#define LEFT    GLFW_KEY_LEFT
#define DOWN    GLFW_KEY_DOWN
#define UP      GLFW_KEY_UP  

#endif
