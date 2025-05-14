#include "vbo.h"


VBO vbo_init(const void* data, unsigned int size){
    VBO self;
    glGenBuffers(1, &self.handle);
    vbo_bind(&self);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    return self;
}

void vbo_bind(VBO* self)
{
    glBindBuffer(GL_ARRAY_BUFFER, self->handle);
}

void vbo_unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}