#include "vao.h"
#include "index_buffer.h"
#include "shader.h"

void renderer_clear();

void renderer_draw_elements(VAO* vao, IndexBuffer* ib, Shader* shader);

void renderer_draw_arrays(VAO* vao, Shader* shader, unsigned int first, unsigned int count);