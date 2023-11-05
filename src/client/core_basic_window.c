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
#include "cards.h"
#include "client.h"
#include "clientWebsocket.h"
#include "gui/textbox.h"
#include "pokerConfig.h"
#include "raylib.h"
#include "renderer.h"
#include "resources.h"
#include <stdio.h>
#include <string.h>


#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
int screenWidth = DEFAULT_WINDOW_WIDTH;
int screenHeight = DEFAULT_WINDOW_HEIGHT;

int connecting = 1;
char buff[2048] = "localhost";
char *port = NULL;
TextBox *textBox = NULL;

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
  StartClient();
  StartRender();

  InitializeConnection();

  Image imBlank = GenImageColor(screenWidth, screenHeight, WHITE);
  texture =
      LoadTextureFromImage(imBlank); // Load blank texture to fill on shader
  UnloadImage(imBlank);

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
  destroy_TextBox(&textBox);
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
void InitializeConnection() {
  if (textBox != NULL)
    destroy_TextBox(&textBox);

  connecting = 1;
  port = NULL;
  Rectangle r;
  r.width = 500;
  r.height = 300;
  r.x = DEFAULT_WINDOW_WIDTH / 2 - r.width / 2;
  r.y = DEFAULT_WINDOW_HEIGHT / 2 - r.height / 2;
  strcpy(buff, "localhost");
  textBox = create_TextBox(r, "Host", buff, 32, 1);
}
void UpdateScale() {
  if (screenWidth != GetRenderWidth() || screenHeight != GetRenderHeight()) {
    screenWidth = GetRenderWidth();
    screenHeight = GetRenderHeight();
    Image imBlank = GenImageColor(screenWidth, screenHeight, WHITE);
    texture =
        LoadTextureFromImage(imBlank); // Load blank texture to fill on shader
    UnloadImage(imBlank);
  }
}
//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void) {
  // Update
  //----------------------------------------------------------------------------------

  UpdateScale();
  enum textbox_button res = update_TextBox(textBox);
  if (connecting) {
    if (res == SUBMIT) {
      if (port == NULL) {
        port = &buff[strlen(buff) + 1];
#ifndef PLATFORM_WEB
        strcpy(port, "27015");
#else
        strcpy(port, "27017");
#endif
        Rectangle r = textBox->backgroundRect;
        destroy_TextBox(&textBox);
        textBox = create_TextBox(r, "Port", port, 6, 1);
      } else {
        websocketConnect(buff, port);
        strcpy(buff, "join;");
        port = buff + 5;
        connecting = 0;
        Rectangle r = textBox->backgroundRect;
        destroy_TextBox(&textBox);
        textBox = create_TextBox(r, "Display Name", port, 64, 1);
      }
    } else if (res == CANCEL && port != NULL) {
      InitializeConnection();
    }
  } else if (textBox != NULL) {
    if (res == SUBMIT) {
      websocketSend(buff);
    }
  } else {
    UpdateRender();
  }

  //----------------------------------------------------------------------------------

  // Draw
  //----------------------------------------------------------------------------------
  BeginDrawing();
  ClearBackground(RAYWHITE);

  /*BeginShaderMode(testShader);
    DrawTextureRec(texture,(Rectangle){0,0,screenWidth,screenHeight},(Vector2){0,0},WHITE);
  EndShaderMode();*/
  Texture tmp = GetCardTexture(0, 0);
  float scale = (screenHeight / 4) / (float)tmp.height;
  if ((screenWidth / 13) / (float)tmp.width < scale)
    scale = (screenWidth / 13) / (float)tmp.width;

  for (int suit = 0; suit < 4; suit++)
    for (int card = 0; card < 13; card++) {
      DrawTextureEx(
          GetCardTexture(suit, card),
          (Vector2){card * scale * tmp.width, suit * scale * tmp.height}, 0,
          scale, WHITE);
    }
  if (textBox != NULL)
    draw_TextBox(textBox);
  else
    DrawRender();
  DrawFPS(10, 10);
  EndDrawing();
  //----------------------------------------------------------------------------------
}
