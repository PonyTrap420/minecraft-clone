#pragma once
#include "../util/util.h"

struct IndexBuffer{
    GLuint handle;
    unsigned int count;
};

struct IndexBuffer ib_init(unsigned int* data, unsigned int count);

void ib_destroy(struct IndexBuffer self);

void ib_bind(struct IndexBuffer* self);

void ib_unbind();