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
#include <time.h>
#include <string.h>                         // Required for: 

#include "texture_manager.h"
#include "button.h"
#include "palette.h"
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

static const int virtualWidth = 1920;
static const int virtualHeight = 1080;

#if defined(PLATFORM_WEB)
static const int screenWidth = 950;
static const int screenHeight = 534;
#else
static const int screenWidth = 1920;
static const int screenHeight = 1080;
#endif

static float scaleX;
static float scaleY;



static RenderTexture2D target = { 0 };  // Render texture to render our game

// TODO: Define global variables here, recommended to make them static
static bool ShouldClose = false;
TextureManager* textureManager;
Button StartButton;
Button QuitButton;
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

	// Screen scaling
	scaleX = (float)screenWidth/virtualWidth;
	scaleY = (float)screenWidth/virtualHeight;


	TraceLog(LOG_INFO,GetWorkingDirectory());
	// Create the texture manager
	textureManager = CreateTextureManager();


	// Load textures
	Manager_LoadTexture(textureManager, "logo", "resources/logo.png");

	// TODO: Load resources / Initialize variables at this point

	// Render texture to draw full screen, enables screen scaling
	// NOTE: If screen is scaled, mouse input should be scaled proportionally
	target = LoadRenderTexture(virtualWidth, virtualHeight);
	SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
	SetTargetFPS(60);     // Set our game frames-per-second
	//--------------------------------------------------------------------------------------
	// Main game loop
	while (!WindowShouldClose() && !ShouldClose)    // Detect window close button
	{
		UpdateDrawFrame();
	}
#endif

	// De-Initialization
	//--------------------------------------------------------------------------------------
	UnloadRenderTexture(target);

	// Unload textures and clean up
	Manager_UnloadTextures(textureManager);

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

float LogoScreenTimer = 0.0f;
float LogoScreenDuration = 2.0f;
void UpdateDrawLogoScreen(void)
{
	LogoScreenTimer += GetFrameTime();
	//Update
	if (LogoScreenTimer >= LogoScreenDuration)
	{
		LogoScreenTimer = 0.0f;
		SwitchScreen(SCREEN_TITLE);  // Switch to Title screen
	}
	//Draw to texture
	BeginTextureMode(target);
	ClearBackground(RAYWHITE);
	
	
	Texture2D logo = Manager_GetTexture(textureManager, "logo");
	Vector2 pos;
	pos.x = virtualWidth / 2.0f - (logo.width / 2.0f);
	pos.y = virtualHeight / 2.0f - (logo.height / 2.0f);

	Color cornFlowerBlue = {.r = 100, .g = 149, .b = 237, .a = 255 };
	DrawRectangle(0,0,virtualWidth, 700*scaleY,cornFlowerBlue);

	Color green = {.r=106,.g=190, .b = 48,.a = 255};
	DrawRectangle(0,700 * scaleY,virtualWidth, 32*scaleY,green);

	Color brown = {.r=102,.g=57, .b = 49,.a = 255};
	DrawRectangle(0,732*scaleY,virtualWidth, 1000 * scaleY,brown);

	DrawTextureEx(logo,(Vector2){pos.x * scaleX,pos.y * scaleY},0.0f,scaleX, WHITE);

	DrawRectangleLinesEx( 
		(Rectangle){virtualWidth / 2.0f - (logo.width / 2.0),
		virtualHeight / 2.0f - (logo.height / 2.0),
		logo.width,
		logo.height},
		15.0f,
		BLACK);

	DrawText("Made By Rhetorical", 815 * scaleX, 800 * scaleY, 30 * scaleX, BLACK);
	DrawText(TextFormat("%.1f",LogoScreenDuration - LogoScreenTimer),15 * scaleX,1050 * scaleY,30 * scaleX,BLACK);
	EndTextureMode();
}


void UpdateDrawTitleScreen(void)
{
	StartButton = CreateButton(
		virtualWidth / 2.0f - 100 * scaleX,
		virtualHeight / 2.0f + (350 - 100) * scaleY,
		200 * scaleX,
		50*scaleY,
		GRAY,
		LIGHTGRAY,
		DARKGRAY);

	QuitButton = CreateButton(
		virtualWidth / 2.0f - 100 * scaleX,
		virtualHeight / 2.0f + (350 - 25) * scaleY,
		200 * scaleX,
		50 * scaleY,
		GRAY,
		LIGHTGRAY,
		DARKGRAY);

	UpdateButton(&StartButton);
	UpdateButton(&QuitButton);
	
	if(StartButton.isPressed)
	{
		SwitchScreen(SCREEN_GAMEPLAY);
	}
	if(QuitButton.isPressed)
	{
		ShouldClose = true;
	}

	//Draw to texture
	BeginTextureMode(target);
	ClearBackground(PALETTE_JUNGLE_GREEN);

	DrawText("PROTO-CALL", virtualWidth / 2.0f - 792 * scaleX, 200 * scaleY, 240 * scaleX, BLACK);
	DrawRectangleLinesEx((Rectangle) { 0, 0, virtualWidth, virtualHeight}, 16 * scaleX, PALETTE_MOSS_GREEN);
	DrawButton(&StartButton, "Start");
	DrawButton(&QuitButton, "Quit");

	EndTextureMode();
}

void UpdateDrawGameplayScreen(void)
{


	//Draw to texture
	BeginTextureMode(target);
	ClearBackground(PALETTE_JUNGLE_GREEN);

	DrawText("Gameplay Screen", 150 * scaleX, 140 * scaleY, 30 * scaleX, BLACK);
	DrawRectangleLinesEx((Rectangle) { 0, 0, virtualWidth, virtualHeight}, 16 * scaleX, PALETTE_MOSS_GREEN);

	EndTextureMode();
}

void UpdateDrawEndingScreen(void)
{
	if (IsKeyPressed(KEY_ENTER))
	{
		SwitchScreen(SCREEN_LOGO);  // Loop back to Logo screen
	}
	//Draw to texture
	BeginTextureMode(target);
	ClearBackground(RAYWHITE);

	DrawText("Ending Screen", 150 * scaleX, 140 * scaleY, 30 * scaleX, BLACK);
	DrawRectangleLinesEx((Rectangle) { 0, 0, virtualWidth, virtualHeight}, 16 * scaleX, BLACK);

	EndTextureMode();
}

void SwitchScreen(GameScreen screen)
{
	currentScreen = screen;
	switch (currentScreen)
	{
	case SCREEN_LOGO: 
	{
	}
	break;
	case SCREEN_TITLE:
	{
	}
	break;
	case SCREEN_GAMEPLAY: 
	{
	}
	break;
	case SCREEN_ENDING:
	{
	}
	break;
	default: break;
	}
}
