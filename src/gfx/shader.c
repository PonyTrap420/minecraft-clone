#include "shader.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <GL/glew.h>

static shader_source parse_shader_single(const char* file_path);
static shader_source parse_shader(const char* folder_path);
static unsigned int compile_shader(unsigned int type, const char* source);
static unsigned int create_shader(const char* vertex_shader, const char* fragment_shader);
static int get_uniform_location(unsigned int program, const char* name);

Shader* shader_create(const char* file_path, shader_source_type type) {
    Shader* self = (Shader*)malloc(sizeof(Shader));
    self->file_path = strdup(file_path);
    self->source_type = type;

    shader_source source;
    if(type == shader_single_file)
        source = parse_shader_single(file_path);
    else
        source = parse_shader(file_path);

    self->handle = create_shader(source.vertex, source.fragment);

    free(source.vertex);
    free(source.fragment);

    return self;
}

void shader_destroy(Shader* self) {
    if (!self) return;
    glDeleteProgram(self->handle);
    free(self->file_path);
    free(self);
}

void shader_bind(const Shader* self) {
    glUseProgram(self->handle);
}

void shader_unbind() {
    glUseProgram(0);
}

void shader_set_uniform_1i(Shader* self, const char* name, int value) {
    glUniform1i(get_uniform_location(self->handle, name), value);
}

void shader_set_uniform_1f(Shader* self, const char* name, float value) {
    glUniform1f(get_uniform_location(self->handle, name), value);
}

void shader_set_uniform_3f(Shader* self, const char* name, float v0, float v1, float v2) {
    glUniform3f(get_uniform_location(self->handle, name), v0, v1, v2);
}

void shader_set_uniform_4f(Shader* self, const char* name, float v0, float v1, float v2, float v3) {
    glUniform4f(get_uniform_location(self->handle, name), v0, v1, v2, v3);
}

void shader_set_uniform_mat4f(Shader* self, const char* name, const float* matrix) {
    glUniformMatrix4fv(get_uniform_location(self->handle, name), 1, GL_FALSE, matrix);
}

static shader_source parse_shader_single(const char* file_path) {
    shader_source source = { 0 };

    char* content = read_file(file_path); 
    const char* vertex_token = "#shader vertex";
    const char* fragment_token = "#shader fragment";

    const char* vertex_start = strstr(content, vertex_token);
    const char* fragment_start = strstr(content, fragment_token);

    if (vertex_start && fragment_start) {
        vertex_start += strlen(vertex_token);
        const char* fragment_header_end = fragment_start + strlen(fragment_token);

        int vertex_length = fragment_start - vertex_start;
        int fragment_length = strlen(fragment_header_end);

        source.vertex = (char*)malloc(vertex_length + 1);
        source.fragment = (char*)malloc(fragment_length + 1);

        strncpy(source.vertex, vertex_start, vertex_length);
        source.vertex[vertex_length] = '\0';
        strcpy(source.fragment, fragment_header_end);
    }

    free(content);
    return source;
}

shader_source parse_shader(const char* folder_path) {
    const char* folder_name = strrchr(folder_path, '/');
    folder_name = folder_name ? folder_name + 1 : folder_path;

    char vertex_path[512];
    char fragment_path[512];

    snprintf(vertex_path, sizeof(vertex_path), "%s/%s.vert", folder_path, folder_name);
    snprintf(fragment_path, sizeof(fragment_path), "%s/%s.frag", folder_path, folder_name);

    char* vertex_src = read_file(vertex_path);
    char* fragment_src = read_file(fragment_path);

    if (!vertex_src || !fragment_src) {
        fprintf(stderr, "Error reading shader files.\n");
        exit(EXIT_FAILURE);
    }

    shader_source result = {
        .vertex = vertex_src,
        .fragment = fragment_src
    };

    return result;
}

static unsigned int compile_shader(unsigned int type, const char* source) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length);
        glGetShaderInfoLog(id, length, &length, message);
        fprintf(stderr, "Failed to compile %s shader:\n%s\n",
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);
        free(message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int create_shader(const char* vertex_shader, const char* fragment_shader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

static int get_uniform_location(unsigned int program, const char* name) {
    //implement caching
    return glGetUniformLocation(program, name);
}
