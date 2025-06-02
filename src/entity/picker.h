#pragma once

#include "camera.h"
#include "../world/world.h"
#include <cglm/cglm.h>
#include <stdbool.h>

typedef struct {
    int x, y, z;
    int face;
    bool hit;
} BlockHit;

BlockHit pick_block(World* world, Camera* camera, float max_distance);
int picker_get_wireframe();
void picker_init_wireframe_cube();