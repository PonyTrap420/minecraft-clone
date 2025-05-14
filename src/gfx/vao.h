#pragma once
#include "../util/util.h"
#include "vbo.h"
#include "vertex_buffer_layout.h"

struct VAO
{
    GLuint handle;
};

struct VAO vao_init();

void vao_destroy(struct VAO self);

void vao_addbuffer(struct VAO self, struct VBO vbo, struct VertexBufferLayout* layout);

void vao_bind(struct VAO self);

void vao_unbind();