#pragma once
#include "../util/util.h"

typedef struct VBO
{
    GLuint handle;
} VBO;

VBO vbo_init(const void* data, unsigned int size);

void vbo_bind(VBO* self);

void vbo_unbind();