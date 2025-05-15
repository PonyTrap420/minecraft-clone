#include "renderer.h"

void gl_clear_error()
{
    while (glGetError());
}

bool gl_log_call(const char* func, const char* file, int line) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        printf("[OpenGL Error] (%u) on %s %s:%d\n", error, func, file, line);
        return false;
    }
    return true;
}

void renderer_clear(){
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_draw_elements(VAO* vao, IndexBuffer* ib, Shader* shader){
    shader_bind(shader);
    vao_bind(vao);
    ib_bind(ib);
    
    GLCall(glDrawElements(GL_TRIANGLES, ib->count, GL_UNSIGNED_INT, 0));
}

void renderer_draw_arrays(VAO* vao, Shader* shader, unsigned int first, unsigned int count){
    shader_bind(shader);
    vao_bind(vao);

    GLCall(glDrawArrays(GL_TRIANGLES, first, count));
}