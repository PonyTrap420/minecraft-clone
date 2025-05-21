#include <stdio.h>
#include "../util/assets.h"
#include "../util/config.h"
#include "../util/util.h"

#include "texture.h"
#include "atlas.h"

#define BLOCKS_ATLAS TEXTURE_DIR "block_atlas"

void texture_init(){
    if (is_directory_empty(MINECRAFT_DIR)) {
        printf("Initializing textures...\n");
        char jar_path[MAX_PATH];
        if (find_latest_minecraft_jar(jar_path, sizeof(jar_path))) {
            printf("Using Minecraft JAR: %s\n", jar_path);
            extract_minecraft_assets(jar_path, MINECRAFT_DIR);
        } else {
            printf("No Minecraft version with .jar found.\n");
        }
    }

    char atlas_path[MAX_PATH];
    sprintf(atlas_path, "%satlases/blocks.json", MINECRAFT_DIR);
    parse_atlas(atlas_path);
    build_atlas(BLOCKS_ATLAS, MIP_LEVEL0);
}

Texture* texture_create(const char* path, TextureType type) {
    Texture* texture = (Texture*)malloc(sizeof(Texture));
    if (!texture) return NULL;

    texture->handle = 0;
    texture->localBuffer = NULL;
    texture->width = 0;
    texture->height = 0;
    texture->bpp = 0;
    texture->type = type;

    texture->filePath = strdup(path);

    stbi_set_flip_vertically_on_load(1);
    texture->localBuffer = stbi_load(path, &texture->width, &texture->height, &texture->bpp, 4);

    GLCall(glGenTextures(1, &texture->handle));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture->handle));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->localBuffer));
    glGenerateMipmap(GL_TEXTURE_2D);
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (texture->localBuffer) {
        stbi_image_free(texture->localBuffer);
        texture->localBuffer = NULL;
    }

    return texture;
}

void texture_destroy(Texture* texture) {
    if (!texture) return;

    GLCall(glDeleteTextures(1, &texture->handle));
    free(texture->filePath);
    free(texture);
}

void texture_bind(const Texture* texture, unsigned int slot) {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture->handle));
}

void texture_unbind(void) {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}