#include "vertex_buffer_layout.h"
#include <stdlib.h>

unsigned int vbe_get_size_of_type(unsigned int type) {
    switch (type) {
        case GL_FLOAT:         return 4;
        case GL_UNSIGNED_INT:  return 4;
        case GL_UNSIGNED_BYTE: return 1;
    }
    ASSERT(0 && "Unknown GL type");
    return 0;
}

VertexBufferLayout* vbl_init() {
    VertexBufferLayout* layout = (VertexBufferLayout*)malloc(sizeof(VertexBufferLayout));
    layout->elements = NULL;
    layout->element_count = 0;
    layout->capacity = 0;
    layout->stride = 0;
    return layout;
}

void vbl_free(VertexBufferLayout* layout) {
    free(layout->elements);
    layout->elements = NULL;
    layout->element_count = 0;
    layout->capacity = 0;
    layout->stride = 0;
}

static bool vbl_push(VertexBufferLayout* layout, unsigned int type, unsigned int count, unsigned char normalized) {
    if (layout->element_count >= layout->capacity) {
        size_t new_capacity = layout->capacity == 0 ? 4 : layout->capacity * 2;
        VertexBufferElement* new_elements = realloc(layout->elements, new_capacity * sizeof(VertexBufferElement));
        if (!new_elements) return false;
        layout->elements = new_elements;
        layout->capacity = new_capacity;
    }

    layout->elements[layout->element_count++] = (VertexBufferElement){
        .type = type,
        .count = count,
        .normalized = normalized
    };

    layout->stride += count * vbe_get_size_of_type(type);
    return true;
}

bool vbl_push_float(VertexBufferLayout* layout, unsigned int count) {
    return vbl_push(layout, GL_FLOAT, count, GL_FALSE);
}

bool vbl_push_uint(VertexBufferLayout* layout, unsigned int count) {
    return vbl_push(layout, GL_UNSIGNED_INT, count, GL_FALSE);
}

bool vbl_push_uchar(VertexBufferLayout* layout, unsigned int count) {
    return vbl_push(layout, GL_UNSIGNED_BYTE, count, GL_TRUE);
}
