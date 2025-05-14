#include "vao.h"

struct VAO vao_init(){
    struct VAO self;
    glGenVertexArrays(1, &self.handle);
    return self;
}

void vao_destroy(const struct VAO self){
    glDeleteVertexArrays(1, &self.handle);
}

void vao_addbuffer(struct VAO self, struct VBO vbo, struct VertexBufferLayout* layout)
{
    vao_bind(self);
    vbo_bind(&vbo);

    VertexBufferElement* elements = layout->elements;
    unsigned int offset = 0;
    for(unsigned int i = 0; i < layout->element_count; i++){
        VertexBufferElement element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout->stride, (const void*)(intptr_t)offset);
        offset += element.count * vbe_get_size_of_type(element.type);
    }

    vbl_free(layout);
}

void vao_bind(const struct VAO self){
    glBindVertexArray(self.handle);
}

void vao_unbind(){
    glBindVertexArray(0);
}