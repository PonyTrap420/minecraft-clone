#pragma once

#include <stdbool.h>
#include <cglm/cglm.h>
#include "camera.h"
#include "../world/world.h"

#define GRAVITY -23
#define JUMP 8.0f
#define HEIGHT 1.8f
#define SNEAK_HEIGHT 1.1f

typedef struct Player {
    Camera camera;
    
    float height;
    float width;
    
    vec3 velocity; 
    float jumpStrength;
    float moveSpeed;
    
    bool onGround;
    bool isFlying;

    double lastJumpTime;
    bool spacePressedLastFrame;
} Player;

Player* player_get_instance(void);
void player_update(Player* player, float deltaTime, World* world, GLFWwindow* window);
bool player_collides_at(World* world, float x, float y, float z, float width, float height);