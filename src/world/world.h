#include "chunk.h"
#include "block/blocktypes.h"
#include "../gfx/renderer.h"

Chunk* world_init();
void world_prepare(Chunk* chunk);
void world_render(Chunk* chunk, Shader* shader);