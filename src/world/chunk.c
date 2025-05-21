#include "chunk.h"
#include <cglm/cglm.h>
#include "block/blocktypes.h"

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
                    if (is_face_visible(chunk, x, y, z, dx, dy, dz)) {
                           BlockType block = chunk->blocks[x][y][z];

                            // calculate float offset in the buffer (5 floats per vertex)
                            float* ptr = chunk->vertex_data + chunk->vertex_count * 5;

                            // add_face returns number of vertices * 5 floats written
                            int floats_written = add_face(ptr, x, y, z, i, block);

                            // increment vertex_count by number of vertices added (floats_written/5)
                            chunk->vertex_count += floats_written;

                    }
                }
            }
        }
    }
}

int is_face_visible(Chunk* chunk, int x, int y, int z, int dx, int dy, int dz) {
    int nx = x + dx;
    int ny = y + dy;
    int nz = z + dz;

    if (nx < 0 || nx >= CHUNK_SIZE_X ||
        ny < 0 || ny >= CHUNK_SIZE_Y ||
        nz < 0 || nz >= CHUNK_SIZE_Z) {
        return 1;
    }

    return chunk->blocks[nx][ny][nz] == 0;
}