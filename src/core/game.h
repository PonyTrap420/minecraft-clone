#pragma once
#include "time.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "../gfx/shader.h"
#include "../gfx/index_buffer.h"
#include "../input/input.h"
#include "../world/blockmesh.h"
#include "../gfx/renderer.h"
#include "../world/world.h"
#include "../gfx/texture.h"
#include "../gfx/atlas.h"
#include "../entity/camera.h"
#include "../entity/picker.h"

typedef struct Game{
    GLFWwindow* window;
    unsigned int winHeight;
    unsigned int winWidth;

    Time time;   
    bool wireframe;

    World* world;
    Camera* camera;

    Shader* shader_textured;
    Shader* shader_solid;
    BlockHit picked_block;
    BlockMesh* blockmesh;
}Game;

int start_game(Game* self);

int init(Game* self);

void tick(Game* self);

void update();

void render(Game* self);