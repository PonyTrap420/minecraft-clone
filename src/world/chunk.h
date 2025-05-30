#include <inttypes.h>
#include "../gfx/vao.h"
#include <cglm/cglm.h>
#include "blockmesh.h"
#include "../gfx/shader.h"
#include "../gfx/renderer.h"

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 256
#define CHUNK_SIZE_Z 16

#define MAX_VERTS (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z * 36)
typedef struct {
    uint8_t blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
    
    VAO vao;
    int vertex_count;

    float* vertex_data; 
} Chunk;

Chunk* chunk_init(int coordX, int coordY);
void chunk_prepare(Chunk* chunk);
void chunk_render(Chunk* chunk, Shader* shader);
void chunk_generate_mesh(Chunk* chunk);
int is_face_visible(Chunk* chunk, int x, int y, int z, int dx, int dy, int dz);

void chunk_free(Chunk* chunk);