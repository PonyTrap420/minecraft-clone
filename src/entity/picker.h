#pragma once

#include "camera.h"
#include "../world/world.h"
#include <cglm/cglm.h>
#include <stdbool.h>

typedef enum {
    FACE_NONE = -1,
    FACE_FRONT = 0,
    FACE_BACK = 1,
    FACE_LEFT = 2, 
    FACE_RIGHT = 3,
    FACE_BOTTOM = 4,
    FACE_TOP = 5
} BlockFace;

typedef struct {
    bool hit;
    int x, y, z;
    BlockFace face;
    vec3 hit_pos;
} BlockHit;



BlockHit picker_pick_block(World* world, Camera* camera, float max_distance);
void picker_init();
void picker_render(BlockHit picked_block, bool wireframe, float* view, float* projection);