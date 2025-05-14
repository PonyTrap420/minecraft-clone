#pragma once
#include "../util/util.h"

struct VBO
{
    GLuint handle;
};

struct VBO vbo_init(const void* data, unsigned int size);

void vbo_bind(struct VBO* self);

void vbo_unbind();