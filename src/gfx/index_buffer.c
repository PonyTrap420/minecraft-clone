#include "index_buffer.h"

IndexBuffer ib_init(unsigned int* data, unsigned int count){
    IndexBuffer self;
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    glGenBuffers(1, &self.handle);
    ib_bind(&self);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    return self;
}

void ib_destroy(IndexBuffer self){
    glDeleteBuffers(1, &self.handle);
}

void ib_bind(IndexBuffer* self){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->handle);
}

void ib_unbind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}