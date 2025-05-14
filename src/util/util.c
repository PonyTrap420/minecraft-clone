#include "util.h"
#include <sys/stat.h>

char* read_file(const char* filepath) {
    FILE* file = fopen(filepath, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", filepath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed for file: %s\n", filepath);
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0'; // Null-terminate the buffer

    fclose(file);
    return buffer;
}

bool file_exists (char *filename) {
  struct stat buffer;   
  return (stat (filename, &buffer) == 0);
}