#include "world.h"

static void world_allocate_chunks(World* world) {
    world->chunks = malloc(world->size_x * sizeof(Chunk*));
    for (int i = 0; i < world->size_x; i++) {
        world->chunks[i] = malloc(world->size_z * sizeof(Chunk*));
        for (int j = 0; j < world->size_z; j++) {
            Chunk* chunk = chunk_init();
            chunk_prepare(chunk);
            world->chunks[i][j] = chunk;
        }
    }
}

static void world_free_chunks(World* world) {
    for (int i = 0; i < world->size_x; i++) {
        for (int j = 0; j < world->size_z; j++) {
            if (world->chunks[i][j]) {
                chunk_free(world->chunks[i][j]);
            }
        }
        free(world->chunks[i]);
    }
    free(world->chunks);
}

World* world_init(int radius) {
    int diameter = radius * 2 + 1;

    World* world = malloc(sizeof(World));
    world->size_x = diameter;
    world->size_z = diameter;
    world->origin_x = -radius;
    world->origin_z = -radius;

    world_allocate_chunks(world);
    return world;
}

void world_update(World* world, float player_x, float player_z) {
    int player_chunk_x = floor(player_x / CHUNK_SIZE_X);
    int player_chunk_z = floor(player_z / CHUNK_SIZE_Z);

    int dx = player_chunk_x - (world->origin_x + world->size_x / 2);
    int dz = player_chunk_z - (world->origin_z + world->size_z / 2);

    if (abs(dx) >= 1 || abs(dz) >= 1) {
        world_free_chunks(world);

        world->origin_x = player_chunk_x - world->size_x / 2;
        world->origin_z = player_chunk_z - world->size_z / 2;

        world_allocate_chunks(world);
    }
}

void world_render(World* world, Shader* shader)
{   
    for (int i = 0; i < world->size_x; i++) {
        for (int j = 0; j < world->size_z; j++) {
            Chunk* chunk = world->chunks[i][j];
            if (chunk) {
                mat4 model;
                glm_mat4_identity(model);

                vec3 translation = {
                    (world->origin_x + i) * CHUNK_SIZE_X,
                    0.0f,
                    (world->origin_z + j) * CHUNK_SIZE_Z
                };

                glm_translate(model, translation);

                shader_set_uniform_mat4f(shader, "model", (const float*)model);
                chunk_render(chunk, shader);
            }
        }
    }
}