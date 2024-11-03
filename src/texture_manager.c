// texture_manager.c

#include "texture_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TextureManager* CreateTextureManager() {
    TextureManager* manager = (TextureManager*)malloc(sizeof(TextureManager));
    manager->count = 0;
    return manager;
}

void Manager_LoadTexture(TextureManager* manager, const char* name, const char* filePath) {
    if (manager->count >= MAX_TEXTURES) {
        TraceLog(LOG_WARNING,"Texture limit reached, unable to load more textures.");
        return;
    }

    Texture2D texture = LoadTexture(filePath);
    if (texture.id == 0) {
        TraceLog(LOG_WARNING,"Failed to load texture: %s", filePath);
        return;
    }

    manager->textures[manager->count] = texture;
    manager->textureNames[manager->count] = name;
    manager->count++;
}

Texture2D Manager_GetTexture(TextureManager* manager, const char* name) {
    for (int i = 0; i < manager->count; i++) {
        if (strcmp(manager->textureNames[i], name) == 0) {
            return manager->textures[i];
        }
    }
    TraceLog(LOG_WARNING,"Texture not found: %s", name);
    return (Texture2D){0}; // Return a null texture
}

void Manager_UnloadTextures(TextureManager* manager) {
    for (int i = 0; i < manager->count; i++) {
        UnloadTexture(manager->textures[i]);
    }
    free(manager); // Free the manager after unloading textures
}

void Manager_UnloadTexture(TextureManager* manager, const char* name) {
    for (int i = 0; i < manager->count; i++) {
        if (strcmp(manager->textureNames[i], name) == 0) {
            UnloadTexture(manager->textures[i]);
            manager->textures[i] = (Texture2D){0}; // Set to null
            // Shift remaining textures
            for (int j = i; j < manager->count - 1; j++) {
                manager->textures[j] = manager->textures[j + 1];
                manager->textureNames[j] = manager->textureNames[j + 1];
            }
            manager->count--;
            return;
        }
    }
    TraceLog(LOG_WARNING,"Texture not found for unloading: %s", name);
}
