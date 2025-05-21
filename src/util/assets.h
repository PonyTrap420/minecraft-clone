// minecraft_textures.h

#ifndef MINECRAFT_TEXTURES_H
#define MINECRAFT_TEXTURES_H

#include <windows.h>

int is_directory_empty(const char *path);

void get_minecraft_versions_path(char *out, size_t size);

int find_latest_minecraft_jar(char *output_path, size_t size);

int extract_minecraft_assets(const char *jar_path, const char* dest);

#endif
