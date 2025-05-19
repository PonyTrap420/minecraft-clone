#include "world.h"

Chunk* world_init() {
    Chunk* chunk = malloc(sizeof(Chunk));
    if (!chunk) return NULL;

    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int z = 0; z < CHUNK_SIZE_Z; z++) {
            for (int y = 0; y < CHUNK_SIZE_Y; y++) {
                if (y < 64 && y > 60)
                    chunk->blocks[x][y][z] = BLOCK_DIRT;
                else if(y<=60)
                    chunk->blocks[x][y][z] = BLOCK_STONE;
                else 
                    chunk->blocks[x][y][z] = BLOCK_AIR;   
            }
        }
    }

    chunk_generate_mesh(chunk);
    return chunk;
}

void world_prepare(Chunk* chunk){
    chunk->vao = vao_init();
    VBO vbo = vbo_init((float*)chunk->vertices, chunk->vertex_count * sizeof(vec3));

    VertexBufferLayout* layout = vbl_init();
    vbl_push_float(layout, 3);

    vao_addbuffer(chunk->vao, vbo, layout);
}

void world_render(Chunk* chunk, Shader* shader) {
    renderer_draw_arrays(&chunk->vao, shader, 0, chunk->vertex_count);
}