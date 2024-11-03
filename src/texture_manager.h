// texture_manager.h

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "raylib.h"

#define MAX_TEXTURES 128

typedef struct {
    Texture2D textures[MAX_TEXTURES];
    const char* textureNames[MAX_TEXTURES];
    int count;
} TextureManager;

// Function declarations
TextureManager* CreateTextureManager();
void Manager_LoadTexture(TextureManager* manager, const char* name, const char* filePath);
Texture2D Manager_GetTexture(TextureManager* manager, const char* name);
void Manager_UnloadTextures(TextureManager* manager);
void Manager_UnloadTexture(TextureManager* manager, const char* name);

#endif // TEXTURE_MANAGER_H
