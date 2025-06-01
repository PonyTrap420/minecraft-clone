#include "picker.h"
#include <math.h>

#define STEP_SIZE 0.01f

BlockHit pick_block(World* world, Camera* camera, float max_distance) {
    BlockHit result = { .hit = false };

    vec3 pos;
    glm_vec3_copy(camera->cameraPos, pos);

    vec3 dir;
    glm_vec3_copy(camera->cameraFront, dir);
    glm_vec3_normalize(dir);

    float traveled = 0.0f;
    while (traveled < max_distance) {
        int block_x = (int)floorf(pos[0]);
        int block_y = (int)floorf(pos[1]);
        int block_z = (int)floorf(pos[2]);

        int block = world_get_block(world, block_x, block_y, block_z);
        if (block != 0) { // Assume 0 is air
            result.hit = true;
            result.x = block_x;
            result.y = block_y;
            result.z = block_z;
            // Optional: compute face hit
            break;
        }

        // Move forward along ray
        pos[0] += dir[0] * STEP_SIZE;
        pos[1] += dir[1] * STEP_SIZE;
        pos[2] += dir[2] * STEP_SIZE;
        traveled += STEP_SIZE;
    }

    return result;
}
