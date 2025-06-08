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
#include "../gfx/debug.h"
#include "../entity/player.h"

#define GAME_FLAG_WIREFRAME   (1 << 0)
#define GAME_FLAG_SHOW_CHUNKS (1 << 1)
typedef struct Game{
    GLFWwindow* window;
    unsigned int winHeight;
    unsigned int winWidth;

    Time time;   
    uint8_t flags;

    World* world;

    Player* player;
    Camera* camera;

    Shader* shader_textured;
    BlockHit picked_block;
    BlockMesh* blockmesh;
}Game;

int start_game(Game* self);

int init(Game* self);

void tick(Game* self);

void update(Game* self, float deltaTime);

void render(Game* self);