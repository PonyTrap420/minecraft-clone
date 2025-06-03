#include "debug.h"

static unsigned int chunkBorderVAO, chunkBorderVBO, chunkBorderEBO;
Shader* shader;

void init_chunk_border_mesh() {
    shader = shader_create(SHADER_DIR"chunks", shader_separate);

    float vertices[] = {
        // x, y, z
        0, 0, 0, // 0
        1, 0, 0, // 1
        1, 0, 1, // 2
        0, 0, 1, // 3
        0, 1, 0, // 4
        1, 1, 0, // 5
        1, 1, 1, // 6
        0, 1, 1  // 7
    };

    unsigned int indices[] = {
        // bottom
        0, 1, 2, 2, 3, 0,
        // top
        4, 5, 6, 6, 7, 4,
        // front
        0, 1, 5, 5, 4, 0,
        // back
        2, 3, 7, 7, 6, 2,
        // left
        3, 0, 4, 4, 7, 3,
        // right
        1, 2, 6, 6, 5, 1
    };

    glGenVertexArrays(1, &chunkBorderVAO);
    glGenBuffers(1, &chunkBorderVBO);
    glGenBuffers(1, &chunkBorderEBO);

    glBindVertexArray(chunkBorderVAO);

    glBindBuffer(GL_ARRAY_BUFFER, chunkBorderVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkBorderEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void render_chunk_border(int chunk_x, int chunk_z, float* view, float* projection) {
    shader_bind(shader);

    mat4 model;
    glm_mat4_identity(model);

    shader_set_uniform_mat4f(shader, "model", (float*)model);
    shader_set_uniform_mat4f(shader, "view", view);
    shader_set_uniform_mat4f(shader, "projection", projection);

    
    vec3 translation = {
        chunk_x * CHUNK_SIZE_X,
        0.0f,
        chunk_z * CHUNK_SIZE_Z
    };
    glm_translate(model, translation);

    vec3 scale = {
        CHUNK_SIZE_X,
        CHUNK_SIZE_Y,
        CHUNK_SIZE_Z
    };
    glm_scale(model, scale);

    shader_set_uniform_mat4f(shader, "model", (float*)model);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(chunkBorderVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
