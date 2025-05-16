#pragma once

#include "../core/game.h"
#include <GLFW/glfw3.h>
#include "../entity/camera.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);