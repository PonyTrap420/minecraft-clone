#pragma once
#include <cglm/cglm.h>

#define MAX_TEXTURES 2048
#define MAX_PATH_LEN 512

#define MIP_SIZE(level) (16 >> (level))
enum {
    MIP_LEVEL0 = 0,
    MIP_LEVEL1 = 1,
    MIP_LEVEL2 = 2,
    MIP_LEVEL3 = 3,
    MIP_LEVEL4 = 4,
};

typedef struct AtlasTexture{
    char* key;
    int value;

    char path[MAX_PATH_LEN];
} AtlasTexture;

void parse_atlas(const char *json_path);
void build_atlas(const char *output_path, uint8_t mip_level);
void atlas_getuv(const char* block, vec2 out_uv);