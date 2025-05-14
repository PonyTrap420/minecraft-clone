#pragma once

#include <gl/glew.h>
#include <stdbool.h>
#include <stddef.h>
#include "../util/util.h"

typedef struct VertexBufferElement{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
} VertexBufferElement;

unsigned int vbe_get_size_of_type(unsigned int type);

typedef struct VertexBufferLayout{
    VertexBufferElement* elements;
    size_t element_count;
    size_t capacity;
    unsigned int stride;
} VertexBufferLayout;

VertexBufferLayout* vbl_init();
void vbl_free(VertexBufferLayout* layout);

bool vbl_push_float(VertexBufferLayout* layout, unsigned int count);
bool vbl_push_uint(VertexBufferLayout* layout, unsigned int count);
bool vbl_push_uchar(VertexBufferLayout* layout, unsigned int count);