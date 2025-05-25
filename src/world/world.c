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

    int new_origin_x = player_chunk_x - world->size_x / 2;
    int new_origin_z = player_chunk_z - world->size_z / 2;

    int dx = new_origin_x - world->origin_x;
    int dz = new_origin_z - world->origin_z;

    if (dx == 0 && dz == 0) return;

    // Step 1: free chunks that moved out of view
    for (int i = 0; i < world->size_x; i++) {
        for (int j = 0; j < world->size_z; j++) {
            int chunk_x = world->origin_x + i;
            int chunk_z = world->origin_z + j;

            int new_x = chunk_x + dx;
            int new_z = chunk_z + dz;

            if (new_x < new_origin_x || new_x >= new_origin_x + world->size_x ||
                new_z < new_origin_z || new_z >= new_origin_z + world->size_z) {
                chunk_free(world->chunks[i][j]);
                world->chunks[i][j] = NULL;
            }
        }
    }

    Chunk*** new_chunks = malloc(world->size_x * sizeof(Chunk**));
    for (int i = 0; i < world->size_x; i++) {
        new_chunks[i] = malloc(world->size_z * sizeof(Chunk*));
        for (int j = 0; j < world->size_z; j++) {
            new_chunks[i][j] = NULL;

            int src_i = i - dx;
            int src_j = j - dz;

            if (src_i >= 0 && src_i < world->size_x &&
                src_j >= 0 && src_j < world->size_z) {
                new_chunks[i][j] = world->chunks[src_i][src_j];
            }
        }
    }

    for (int i = 0; i < world->size_x; i++) {
        free(world->chunks[i]);
    }
    free(world->chunks);

    world->chunks = new_chunks;
    world->origin_x = new_origin_x;
    world->origin_z = new_origin_z;

    for (int i = 0; i < world->size_x; i++) {
        for (int j = 0; j < world->size_z; j++) {
            if (world->chunks[i][j] == NULL) {
                Chunk* chunk = chunk_init();
                chunk_prepare(chunk);
                world->chunks[i][j] = chunk;
            }
        }
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