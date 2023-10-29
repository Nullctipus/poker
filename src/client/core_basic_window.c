/*******************************************************************************************
 *
 *   raylib [core] example - Basic window (adapted for HTML5 platform)
 *
 *   This example is prepared to compile for PLATFORM_WEB and PLATFORM_DESKTOP
 *   As you will notice, code structure is slightly different to the other
 *examples... To compile it for PLATFORM_WEB just uncomment #define PLATFORM_WEB
 *at beginning
 *
 *   This example has been created using raylib 1.3 (www.raylib.com)
 *   raylib is licensed under an unmodified zlib/libpng license (View raylib.h
 *for details)
 *
 *   Copyright (c) 2015 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "core_basic_window.h"
#include "raylib.h"
#include "gui/textbox.h"
#include "resources.h"
#include "pokerConfig.h"
#include <stdio.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
int screenWidth = DEFAULT_WINDOW_WIDTH;
int screenHeight = DEFAULT_WINDOW_HEIGHT;
TextBox *testBox;
char testText[1024];

int frames = 0;
int timeloc;
Shader testShader;
Texture2D texture;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void); // Update and Draw one frame

//----------------------------------------------------------------------------------
// Main Entry Point
//----------------------------------------------------------------------------------
int createWindow() {
  // Initialization
  //--------------------------------------------------------------------------------------
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenWidth, screenHeight, WINDOW_TITLE);
  

  Rectangle r;
  r.width = 500;
  r.height = 300;
  r.x = screenWidth/2 - r.width/2;
  r.y = screenHeight/2 - r.height/2;
  testBox = create_TextBox(r,"Test Title",testText,1024,1);
  Image imBlank = GenImageColor(screenWidth, screenHeight, WHITE);
  texture = LoadTextureFromImage(imBlank);  // Load blank texture to fill on shader
  UnloadImage(imBlank);
  testShader = LoadShaderFromMemory(NULL,(const char*) test_fs);
  timeloc = GetShaderLocation(testShader,"frames");

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    UpdateDrawFrame();
  }
#endif

  // De-Initialization
  //--------------------------------------------------------------------------------------
  destroy_TextBox(&testBox);
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void) {
  // Update
  //----------------------------------------------------------------------------------

  if(screenWidth != GetRenderWidth() || screenHeight != GetRenderHeight()){
    screenWidth = GetRenderWidth();
    screenHeight = GetRenderHeight();
    Image imBlank = GenImageColor(screenWidth, screenHeight, WHITE);
    texture = LoadTextureFromImage(imBlank);  // Load blank texture to fill on shader
    UnloadImage(imBlank);
  }

  frames++;
  SetShaderValue(testShader,timeloc,&frames,SHADER_UNIFORM_INT);
  if(update_TextBox(testBox) != NONE){
    destroy_TextBox(&testBox);
    testBox = NULL;
    printf("TestBox Closed. Text is: %s",testText);
  }
  
  //----------------------------------------------------------------------------------

  // Draw
  //----------------------------------------------------------------------------------
  BeginDrawing();
    ClearBackground(BLACK);
  
    BeginShaderMode(testShader);
      DrawTextureRec(texture,(Rectangle){0,0,screenWidth,screenHeight},(Vector2){0,0},WHITE);
    EndShaderMode();

    draw_TextBox(testBox);

  EndDrawing();
  //----------------------------------------------------------------------------------
}
