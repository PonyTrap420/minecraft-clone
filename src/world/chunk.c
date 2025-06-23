#include "chunk.h"
#include <cglm/cglm.h>
#include "block/blocktypes.h"
#include <noise1234.h>

Chunk* world_get_chunk(World* world, int chunk_x, int chunk_z);

Chunk* chunk_init(World* world, int chunk_x, int chunk_z) {
    Chunk* chunk = malloc(sizeof(Chunk));
    if (!chunk) return NULL;
    chunk->chunk_x = chunk_x;
    chunk->chunk_z = chunk_z;
    chunk->world = world;

    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int z = 0; z < CHUNK_SIZE_Z; z++) {
            for (int y = 0; y < CHUNK_SIZE_Y; y++) {
                int world_x = chunk_x * CHUNK_SIZE_X + x;
                int world_z = chunk_z * CHUNK_SIZE_Z + z;
                
                float h = noise2(world_x * 0.05f, world_z * 0.05f) * 8 + 64;

                if (h < 60) h = 60;
                if (h > 68) h = 68;
                
                if (y < h && y > h-4)
                    chunk->blocks[x][y][z] = BLOCK_DIRT;
                else if(y<=h-4)
                    chunk->blocks[x][y][z] = BLOCK_STONE;
                else 
                    chunk->blocks[x][y][z] = BLOCK_AIR;   
                }
            }
    }

    return chunk;
}


void chunk_generate_mesh(Chunk* chunk) {
    int max_vertices = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z * 6 * 6;
    chunk->vertex_data = malloc(max_vertices * sizeof(float));
    chunk->vertex_count = 0;

    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int z = 0; z < CHUNK_SIZE_Z; z++) {
                if (chunk->blocks[x][y][z] == 0) continue;
                
                int directions[6][3] = {
                    { 0,  0, -1}, // -Z (back)
                    { 0,  0,  1}, // +Z (front)
                    {-1,  0,  0}, // -X (left)
                    { 1,  0,  0}, // +X (right)
                    { 0, -1,  0}, // -Y (bottom)
                    { 0,  1,  0}, // +Y (top)
                };
                
              
                for (int i = 0; i < 6; i++) {
                    int dx = directions[i][0];
                    int dy = directions[i][1];
                    int dz = directions[i][2];
                    if (is_face_visible(chunk->world, chunk, x, y, z, dx, dy, dz)) {
                           BlockType block = chunk->blocks[x][y][z];

                            float* ptr = chunk->vertex_data + chunk->vertex_count * 6;

                            int floats_written = add_face(ptr, x, y, z, i, block);

                            chunk->vertex_count += floats_written;

                    }
                }
            }
        }
    }
}

void chunk_prepare(Chunk* chunk){
    chunk->vao = vao_init();
    VBO vbo = vbo_init(chunk->vertex_data, chunk->vertex_count * 6 * sizeof(float));

    VertexBufferLayout* layout = vbl_init();
    vbl_push_float(layout, 3);
    vbl_push_float(layout, 2);
    vbl_push_float(layout, 1);

    vao_addbuffer(chunk->vao, vbo, layout);
}

void chunk_render(Chunk* chunk, Shader* shader){
    renderer_draw_arrays(&chunk->vao, shader, 0, chunk->vertex_count);
}


int is_face_visible(World* world, Chunk* chunk, int x, int y, int z, int dx, int dy, int dz) {
    int nx = x + dx;
    int ny = y + dy;
    int nz = z + dz;

    if (ny < 0 || ny >= CHUNK_SIZE_Y) {
        return 1;
    }

    if (nx < 0 || nx >= CHUNK_SIZE_X || nz < 0 || nz >= CHUNK_SIZE_Z) {
        int neighbor_chunk_x = chunk->chunk_x + (nx < 0 ? -1 : (nx >= CHUNK_SIZE_X ? 1 : 0));
        int neighbor_chunk_z = chunk->chunk_z + (nz < 0 ? -1 : (nz >= CHUNK_SIZE_Z ? 1 : 0));

        Chunk* target_chunk = world_get_chunk(world, neighbor_chunk_x, neighbor_chunk_z);
       if (!target_chunk) return 1;
        if (target_chunk->chunk_x != neighbor_chunk_x || target_chunk->chunk_z != neighbor_chunk_z) return 1;

        nx = (nx + CHUNK_SIZE_X) % CHUNK_SIZE_X;
        nz = (nz + CHUNK_SIZE_Z) % CHUNK_SIZE_Z;
        return target_chunk->blocks[nx][ny][nz] == BLOCK_AIR;
    }
    return chunk->blocks[nx][ny][nz] == BLOCK_AIR;

}


void chunk_free(Chunk* chunk) {
    if (!chunk) return;

    if (chunk->vertex_data) {
        free(chunk->vertex_data);
        chunk->vertex_data = NULL;
    }

    vao_destroy(chunk->vao);

    free(chunk);
}