#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parson.h"
#include "config.h"
#include <windows.h>
#include "atlas.h"
#include <math.h>
#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_ds.h>
#include <stb_image_resize2.h>

#define BASE_PATH MINECRAFT_DIR "/textures/"

static AtlasTexture* atlas_map = NULL;
static int atlas_size;
int tex_count = 0;

void add_texture(const char* id, const char* path) {
    AtlasTexture t;
    t.key = strdup(id);
    strncpy(t.path, path, MAX_PATH_LEN - 1);
    t.path[MAX_PATH_LEN - 1] = '\0';
    t.value = tex_count++;

    shputs(atlas_map, t);
}

void load_directory_textures(const char *source, const char *prefix) {
    char search_path[MAX_PATH_LEN];
    snprintf(search_path, sizeof(search_path), "%s%s\\*.png", BASE_PATH, source);

    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA(search_path, &fd);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Failed to open directory: %s\n", search_path);
        return;
    }

    do {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char texture_id[MAX_PATH_LEN];
            snprintf(texture_id, sizeof(texture_id), "%s%s", prefix, fd.cFileName);

            char *dot = strrchr(texture_id, '.');
            if (dot) *dot = '\0';

            char full_file_path[MAX_PATH_LEN];
            snprintf(full_file_path, sizeof(full_file_path), "%s%s\\%s", BASE_PATH, source, fd.cFileName);

            add_texture(texture_id, full_file_path);
        }
    } while (FindNextFileA(hFind, &fd));
    FindClose(hFind);
}

void load_single_texture(const char *resource) {
    char full_path[MAX_PATH_LEN];
    snprintf(full_path, sizeof(full_path), "%s%s.png", BASE_PATH, resource);

    add_texture(resource, full_path);
}

void parse_atlas(const char *json_path) {
    JSON_Value *root_val = json_parse_file(json_path);
    if (!root_val) {
        printf("Failed to parse JSON: %s\n", json_path);
        return;
    }

    JSON_Object *root_obj = json_value_get_object(root_val);
    JSON_Array *sources = json_object_get_array(root_obj, "sources");

    size_t count = json_array_get_count(sources);
    for (size_t i = 0; i < count; i++) {
        JSON_Object *source_obj = json_array_get_object(sources, i);
        const char *type = json_object_get_string(source_obj, "type");

        if (strcmp(type, "minecraft:directory") == 0) {
            const char *prefix = json_object_get_string(source_obj, "prefix");
            const char *source = json_object_get_string(source_obj, "source");
            load_directory_textures(source, prefix);
        } else if (strcmp(type, "minecraft:single") == 0) {
            const char *resource = json_object_get_string(source_obj, "resource");
            load_single_texture(resource);
        } else {
            printf("[SKIP] Unsupported type: %s\n", type);
        }
    }

    json_value_free(root_val);
}

void build_atlas(const char *output_path, UINT8 mip_level) {
    int rows = (int)ceil(sqrt(hmlen(atlas_map)));
    int cell_size = MIP_SIZE(mip_level);
    atlas_size = rows * cell_size;

    unsigned char *atlas = calloc(atlas_size * atlas_size * 4, 1);

    for (int i = 0; i < hmlen(atlas_map); ++i) {
        const char* path = atlas_map[i].path;
        int width, height, channels;
        unsigned char *data = stbi_load(path, &width, &height, &channels, 4);
        if (!data) {
            printf("Failed to load %s\n", path);
            continue;
        }

        unsigned char *resized = malloc(cell_size * cell_size * 4);

        stbir_resize(
            data, width, height, 0,
            resized, cell_size, cell_size, 0,
            4,  STBIR_TYPE_UINT8, STBIR_EDGE_CLAMP,STBIR_FILTER_DEFAULT
        );

        int x = (i % rows) * cell_size;
        int y = (i / rows) * cell_size;

        for (int row = 0; row < cell_size; ++row) {
            for (int col = 0; col < cell_size; ++col) {
                int dst_idx = ((y + row) * atlas_size + (x + col)) * 4;
                int src_idx = (row * cell_size + col) * 4;
                memcpy(&atlas[dst_idx], &resized[src_idx], 4);
            }
        }

        stbi_image_free(data);
        free(resized);
    }

    char final_output_path[MAX_PATH_LEN];
    sprintf(final_output_path, "%s_%d.png",output_path, mip_level);
    stbi_write_png(final_output_path, atlas_size, atlas_size, 4, atlas, atlas_size * 4);
    free(atlas);
    printf("Atlas saved to %s\n", output_path);
}

void atlas_getuv(const char* block, vec2 out_uv) {
    out_uv[0] = 0.0f;
    out_uv[1] = 0.0f;

    int index = shgets(atlas_map, block).value;
    if (!index) {
        printf("Texture '%s' not found in atlas.\n", block);
        return;
    }

    int rows = (int)ceil(sqrt(hmlen(atlas_map)));
    float cell_uv = 1.0f / (float)rows;

    int col = index % rows;
    int row = index / rows;

    out_uv[0] = col * cell_uv;
    out_uv[1] = row * cell_uv;
}