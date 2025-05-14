#pragma once
#include "../util/util.h"

typedef struct IndexBuffer{
    GLuint handle;
    unsigned int count;
} IndexBuffer;

IndexBuffer ib_init(unsigned int* data, unsigned int count);

void ib_destroy(IndexBuffer self);

void ib_bind(IndexBuffer* self);

void ib_unbind();