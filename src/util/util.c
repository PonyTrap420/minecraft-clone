#include "util.h"
#include <sys/stat.h>
#include <windows.h>
#include <stdio.h>

int is_directory_empty(const char *path) {
    char search_path[MAX_PATH];
    snprintf(search_path, MAX_PATH, "%s\\*", path);

    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile(search_path, &fd);
    if (hFind == INVALID_HANDLE_VALUE) return 1; // Consider it empty on error

    int is_empty = 1;
    do {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || 
            (strcmp(fd.cFileName, ".") != 0 && strcmp(fd.cFileName, "..") != 0)) {
            is_empty = 0;
            break;
        }
    } while (FindNextFile(hFind, &fd));
    FindClose(hFind);
    return is_empty;
}

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