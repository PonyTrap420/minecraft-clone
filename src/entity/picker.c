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

static unsigned int wireframe_vao, wireframe_vbo, wireframe_ebo;

int picker_get_wireframe(){
    return wireframe_vao;
}

void picker_init_wireframe_cube() {
    float vertices[] = {
        0, 0, 0,
        1, 0, 0,
        1, 1, 0,
        0, 1, 0,
        0, 0, 1,
        1, 0, 1,
        1, 1, 1,
        0, 1, 1,
    };

    unsigned int indices[] = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };

    glGenVertexArrays(1, &wireframe_vao);
    glGenBuffers(1, &wireframe_vbo);
    glGenBuffers(1, &wireframe_ebo);

    glBindVertexArray(wireframe_vao);

    glBindBuffer(GL_ARRAY_BUFFER, wireframe_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wireframe_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}