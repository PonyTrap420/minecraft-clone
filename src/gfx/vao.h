#pragma once
#include "../util/util.h"
#include "vbo.h"
#include "vertex_buffer_layout.h"

typedef struct VAO
{
    GLuint handle;
} VAO;

struct VAO vao_init();

void vao_destroy(VAO self);

void vao_addbuffer(VAO self, VBO vbo, VertexBufferLayout* layout);

void vao_bind(const VAO* self);

void vao_unbind();