#pragma once
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "config.h"

void gl_clear_error();
bool gl_log_call(const char* func, const char* file, int line);

#define ASSERT(x) if (!(x)) __debugbreak()
#define GLCall(x) gl_clear_error();\
    x;\
    ASSERT(gl_log_call(#x, __FILE__, __LINE__))


int is_directory_empty(const char *path);
char* read_file(const char* filepath);
bool file_exists (char *filename);