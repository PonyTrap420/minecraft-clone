#include "vao.h"
#include "index_buffer.h"
#include "shader.h"

void renderer_clear();

void renderer_draw(struct VAO* vao, struct IndexBuffer* ib, Shader* shader);

void renderer_draw(struct VAO* vao, Shader* shader);