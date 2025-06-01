#pragma once

#include "chunk.h"
#include "block/blocktypes.h"
#include "../gfx/renderer.h"
#include <cglm/cglm.h>

typedef struct World{
    Chunk*** chunks;
    int size_x;
    int size_z;
    int origin_x;
    int origin_z;
} World;

World* world_init(int radius);
void world_render(World* world, Shader* shader);
void world_update(World* world, float player_x, float player_z);
Chunk* world_get_chunk(World* world, int chunk_x, int chunk_z);
int world_get_block(World* world, int x, int y, int z);