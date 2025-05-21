#pragma once
#include <stb_image.h>

typedef enum {
    TEXTURE_DIFFUSE,
    TEXTURE_SPECULAR,
    TEXTURE_ROUGHNESS,
    TEXTURE_HEIGHT
} TextureType;

typedef struct {
    unsigned int handle;
    unsigned char* localBuffer;
    int width;
    int height;
    int bpp;
    TextureType type;
    char* filePath;
} Texture;

Texture* texture_create(const char* path, TextureType type);

void texture_destroy(Texture* texture);

void texture_init();
void texture_bind(const Texture* texture, unsigned int slot);
void texture_unbind(void);
unsigned int texture_get_handle(const Texture* texture);
int texture_get_width(const Texture* texture);
int texture_get_height(const Texture* texture);