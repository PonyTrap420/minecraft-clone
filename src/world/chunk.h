#include <inttypes.h>
#include "../gfx/vao.h"
#include <cglm/cglm.h>

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 256
#define CHUNK_SIZE_Z 16

#define MAX_VERTS (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z * 36)

typedef struct {
    uint8_t blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
    
    VAO vao;
    int vertex_count;
    vec3* vertices;
} Chunk;

void chunk_generate_mesh(Chunk* chunk);
int is_face_visible(Chunk* chunk, int x, int y, int z, int dx, int dy, int dz);
int add_face(vec3* out, int x, int y, int z, int face);