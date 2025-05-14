#include "../util/util.h"

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