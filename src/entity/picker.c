#include "picker.h"
#include <math.h>

#define STEP_SIZE 0.01f

BlockHit picker_pick_block(World* world, Camera* camera, float max_distance) {
    BlockHit result = { .hit = false, .face = FACE_NONE };

    vec3 pos;
    glm_vec3_copy(camera->cameraPos, pos);

    vec3 dir;
    glm_vec3_copy(camera->cameraFront, dir);
    glm_vec3_normalize(dir);

    vec3 prev_pos;
    glm_vec3_copy(pos, prev_pos);

    float traveled = 0.0f;
    
    while (traveled < max_distance) {
        int block_x = (int)floorf(pos[0]);
        int block_y = (int)floorf(pos[1]);
        int block_z = (int)floorf(pos[2]);

        int block = world_get_block(world, block_x, block_y, block_z);
        if (block != BLOCK_AIR) {
            result.hit = true;
            result.x = block_x;
            result.y = block_y;
            result.z = block_z;
            glm_vec3_copy(pos, result.hit_pos);

            vec3 block_center = {block_x + 0.5f, block_y + 0.5f, block_z + 0.5f};
            vec3 hit_offset;
            glm_vec3_sub(pos, block_center, hit_offset);

            float abs_x = fabsf(hit_offset[0]);
            float abs_y = fabsf(hit_offset[1]);
            float abs_z = fabsf(hit_offset[2]);

            if (abs_x >= abs_y && abs_x >= abs_z) {
                // Hit on X face
                result.face = (hit_offset[0] > 0) ? FACE_RIGHT : FACE_LEFT;
            } else if (abs_y >= abs_x && abs_y >= abs_z) {
                // Hit on Y face
                result.face = (hit_offset[1] > 0) ? FACE_TOP : FACE_BOTTOM;
            } else {
                // Hit on Z face
                result.face = (hit_offset[2] > 0) ? FACE_BACK : FACE_FRONT;
            }

            break;
        }

        // Store previous position before moving
        glm_vec3_copy(pos, prev_pos);

        // Move forward along ray
        pos[0] += dir[0] * STEP_SIZE;
        pos[1] += dir[1] * STEP_SIZE;
        pos[2] += dir[2] * STEP_SIZE;
        traveled += STEP_SIZE;
    }

    return result;
}

static unsigned int wireframe_vao, wireframe_vbo, wireframe_ebo;
Shader* shader_solid;

void picker_init() {
    shader_solid = shader_create(SHADER_DIR"solid", shader_separate);

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

void picker_render(BlockHit picked_block, bool wireframe, float* view, float* projection){
    glDisable(GL_POLYGON_OFFSET_FILL);
    shader_bind(shader_solid);
    shader_set_uniform_mat4f(shader_solid, "view", view);
    shader_set_uniform_mat4f(shader_solid, "projection", projection);
    shader_set_uniform_3f(shader_solid, "color", 0.0f, 0.0f, 0.0f); // black

    mat4 model;
    glm_mat4_identity(model);

    // Translate to block position
    glm_translate(model, (vec3){
        picked_block.x,
        picked_block.y,
        picked_block.z
    });
    
    shader_set_uniform_mat4f(shader_solid, "model", (float*)model);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(2.5f);

    glBindVertexArray(wireframe_vao);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    glLineWidth(1.0f);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);
}