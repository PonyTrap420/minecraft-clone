#pragma once

#include "shader.h"
#include "../world/chunk.h"
#include <cglm/cglm.h>

void init_chunk_border_mesh();
void render_chunk_border(int chunk_x, int chunk_z, float* view, float* projection);