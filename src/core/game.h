#pragma once
#include "time.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
typedef struct Game{
    GLFWwindow* window;
    unsigned int winHeight;
    unsigned int winWidth;

    Time time;   
    bool wireframe;
}Game;

int start_game(Game* self);

int init(Game* self);

void tick(Game* self);

void update();

void render();