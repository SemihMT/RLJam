/*******************************************************************************************
*
*   raylib gamejam template
*
*   Template originally created with raylib 4.5-dev, last time updated with raylib 5.0
*
*   Template licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#if defined(PLATFORM_WEB)
#define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
#include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for: 

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum
{
	SCREEN_LOGO = 0,
	SCREEN_TITLE,
	SCREEN_GAMEPLAY,
	SCREEN_ENDING
} GameScreen;

// TODO: Define your custom data types here

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static const int screenWidth = 1280;
static const int screenHeight = 720;

static RenderTexture2D target = { 0 };  // Render texture to render our game

// TODO: Define global variables here, recommended to make them static

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);      // Update and Draw one frame
GameScreen currentScreen = SCREEN_LOGO;

// Function prototypes for screen updates
void UpdateDrawLogoScreen(void);
void UpdateDrawTitleScreen(void);
void UpdateDrawGameplayScreen(void);
void UpdateDrawEndingScreen(void);

// Function prototype for switching screen
void SwitchScreen(GameScreen screen);
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
#if !defined(_DEBUG)
	SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messages
#endif

	// Initialization
	//--------------------------------------------------------------------------------------
	InitWindow(screenWidth, screenHeight, "PROTO-CALL");

	// TODO: Load resources / Initialize variables at this point

	// Render texture to draw full screen, enables screen scaling
	// NOTE: If screen is scaled, mouse input should be scaled proportionally
	target = LoadRenderTexture(screenWidth, screenHeight);
	SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
	SetTargetFPS(60);     // Set our game frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button
	{
		UpdateDrawFrame();
	}
#endif

	// De-Initialization
	//--------------------------------------------------------------------------------------
	UnloadRenderTexture(target);

	// TODO: Unload all loaded resources at this point

	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}

//--------------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------------
// Update and draw frame
void UpdateDrawFrame(void)
{
	// Update
	// Update the current screen
	switch (currentScreen)
	{
	case SCREEN_LOGO: UpdateDrawLogoScreen(); break;
	case SCREEN_TITLE: UpdateDrawTitleScreen(); break;
	case SCREEN_GAMEPLAY: UpdateDrawGameplayScreen(); break;
	case SCREEN_ENDING: UpdateDrawEndingScreen(); break;
	default: break;
	}

	// Render to screen (main framebuffer)
	BeginDrawing();
	ClearBackground(RAYWHITE);

	// Draw render texture to screen, scaled if required
	DrawTexturePro(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle) { 0, 0, (float)target.texture.width, (float)target.texture.height }, (Vector2) { 0, 0 }, 0.0f, WHITE);

	// TODO: Draw everything that requires to be drawn at this point, maybe UI?

	EndDrawing();
	//----------------------------------------------------------------------------------  
}

void UpdateDrawLogoScreen(void)
{
	//Update
	if (IsKeyPressed(KEY_ENTER))
	{
		SwitchScreen(SCREEN_TITLE);  // Switch to Title screen
	}
	//Draw to texture
	BeginTextureMode(target);
	ClearBackground(RAYWHITE);

	DrawText("Logo Screen", 150, 140, 30, BLACK);
	DrawRectangleLinesEx((Rectangle) { 0, 0, screenWidth, screenHeight }, 16, BLACK);

	EndTextureMode();
}

void UpdateDrawTitleScreen(void)
{
	if (IsKeyPressed(KEY_ENTER))
	{
		SwitchScreen(SCREEN_GAMEPLAY);  // Switch to Gameplay screen
	}
	//Draw to texture
	BeginTextureMode(target);
	ClearBackground(RAYWHITE);

	DrawText("Title Screen", 150, 140, 30, BLACK);
	DrawRectangleLinesEx((Rectangle) { 0, 0, screenWidth, screenHeight }, 16, BLACK);

	EndTextureMode();
}

void UpdateDrawGameplayScreen(void)
{
	 if (IsKeyPressed(KEY_ENTER)) {
        SwitchScreen(SCREEN_ENDING);  // Switch to Ending screen
    }

	//Draw to texture
	BeginTextureMode(target);
	ClearBackground(RAYWHITE);

	DrawText("Gameplay Screen", 150, 140, 30, BLACK);
	DrawRectangleLinesEx((Rectangle) { 0, 0, screenWidth, screenHeight }, 16, BLACK);

	EndTextureMode();
}

void UpdateDrawEndingScreen(void)
{
	 if (IsKeyPressed(KEY_ENTER)) {
        SwitchScreen(SCREEN_LOGO);  // Loop back to Logo screen
    }
	//Draw to texture
	BeginTextureMode(target);
	ClearBackground(RAYWHITE);

	DrawText("Ending Screen", 150, 140, 30, BLACK);
	DrawRectangleLinesEx((Rectangle) { 0, 0, screenWidth, screenHeight }, 16, BLACK);

	EndTextureMode();
}

void SwitchScreen(GameScreen screen)
{
	currentScreen = screen;
}
