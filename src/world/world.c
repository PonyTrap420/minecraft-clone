#include "world.h"

static void world_allocate_chunks(World* world) {
    world->chunks = malloc(world->size_x * sizeof(Chunk*));
    for (int i = 0; i < world->size_x; i++) {
        world->chunks[i] = malloc(world->size_z * sizeof(Chunk*));
        for (int j = 0; j < world->size_z; j++) {
            int chunk_x = world->origin_x + i;
            int chunk_z = world->origin_z + j;
            Chunk* chunk = chunk_init(world, chunk_x,chunk_z);
            world->chunks[i][j] = chunk;
        }
    }
    for (int i = 0; i < world->size_x; i++) {
        for (int j = 0; j < world->size_z; j++) {
            Chunk* chunk = world->chunks[i][j];
            chunk_generate_mesh(chunk);
            chunk_prepare(chunk); 
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

Chunk* world_get_chunk(World* world, int chunk_x, int chunk_z) {
    int i = chunk_x - world->origin_x;
    int j = chunk_z - world->origin_z;

    if (i < 0 || i >= world->size_x || j < 0 || j >= world->size_z)
        return NULL;

    return world->chunks[i][j];
}

void world_update(World* world, float player_x, float player_z) {
    int player_chunk_x = floor(player_x / CHUNK_SIZE_X);
    int player_chunk_z = floor(player_z / CHUNK_SIZE_Z);

    int new_origin_x = player_chunk_x - world->size_x / 2;
    int new_origin_z = player_chunk_z - world->size_z / 2;

    int dx = new_origin_x - world->origin_x;
    int dz = new_origin_z - world->origin_z;

    if (dx == 0 && dz == 0) return;

    Chunk*** new_chunks = malloc(world->size_x * sizeof(Chunk**));
    bool** is_new = malloc(world->size_x * sizeof(bool*));
    for (int i = 0; i < world->size_x; i++) {
        new_chunks[i] = malloc(world->size_z * sizeof(Chunk*));
        is_new[i] = malloc(world->size_z * sizeof(bool));
        for (int j = 0; j < world->size_z; j++) {
            int chunk_x = new_origin_x + i;
            int chunk_z = new_origin_z + j;

            Chunk* old_chunk = world_get_chunk(world, chunk_x, chunk_z);
            if (old_chunk) {
                new_chunks[i][j] = old_chunk;
                is_new[i][j] = false;
            } else {
                Chunk* chunk = chunk_init(world, chunk_x, chunk_z);
                new_chunks[i][j] = chunk;
                is_new[i][j] = true;
            }
        }
    }

    for (int i = 0; i < world->size_x; i++) {
        for (int j = 0; j < world->size_z; j++) {
            int old_chunk_x = world->origin_x + i;
            int old_chunk_z = world->origin_z + j;

            Chunk* reused = world_get_chunk(&(World){
                .chunks = new_chunks,
                .origin_x = new_origin_x,
                .origin_z = new_origin_z,
                .size_x = world->size_x,
                .size_z = world->size_z
            }, old_chunk_x, old_chunk_z);

            if (world->chunks[i][j] && reused != world->chunks[i][j]) {
                chunk_free(world->chunks[i][j]);
            }
        }
        free(world->chunks[i]);
    }
    free(world->chunks);

    world->chunks = new_chunks;
    world->origin_x = new_origin_x;
    world->origin_z = new_origin_z;

    for (int i = 0; i < world->size_x; i++) {
        for (int j = 0; j < world->size_z; j++) {
            if (!is_new[i][j]) continue;

            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni < 0 || nj < 0 || ni >= world->size_x || nj >= world->size_z) continue;

                    Chunk* chunk = world->chunks[ni][nj];
                    if (chunk) {
                        chunk_generate_mesh(chunk);
                        chunk_prepare(chunk);
                    }
                }
            }
        }
        free(is_new[i]);
    }
    free(is_new);
}

int world_get_block(World* world, int x, int y, int z) {
    if (y < 0 || y >= CHUNK_SIZE_Y) return BLOCK_AIR;

    int chunk_x = x / CHUNK_SIZE_X;
    int chunk_z = z / CHUNK_SIZE_Z;

    if (x < 0 && x % CHUNK_SIZE_X != 0) chunk_x -= 1;
    if (z < 0 && z % CHUNK_SIZE_Z != 0) chunk_z -= 1;

    Chunk* chunk = world_get_chunk(world, chunk_x, chunk_z);
    if (!chunk) return 0;

    int local_x = x - chunk_x * CHUNK_SIZE_X;
    int local_z = z - chunk_z * CHUNK_SIZE_Z;

    return chunk->blocks[local_x][y][local_z];
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