#pragma once
#include "../util/util.h"

typedef enum {
    shader_single_file,
    shader_separate
} shader_source_type;

typedef struct {
    char* vertex;
    char* fragment;
} shader_source;

typedef struct {
    GLuint handle;
    char* file_path;
    shader_source_type source_type;
} Shader;

Shader* shader_create(const char* file_path, shader_source_type type);
void shader_destroy(Shader* self);

void shader_bind(const Shader* self);
void shader_unbind();

void shader_set_uniform_1i(Shader* self, const char* name, int value);
void shader_set_uniform_1f(Shader* self, const char* name, float value);
void shader_set_uniform_3f(Shader* self, const char* name, float v0, float v1, float v2);
void shader_set_uniform_4f(Shader* self, const char* name, float v0, float v1, float v2, float v3);
void shader_set_uniform_mat4f(Shader* self, const char* name, const float* matrix); // 4x4 matrix as 1D array
