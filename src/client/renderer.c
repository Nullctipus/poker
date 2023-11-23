#include "renderer.h"
#include "../common/game.h"
#include "cards.h"
#include "client.h"
#include "core_basic_window.h"
#include "gui/button.h"
#include "gui/gui.h"
#include "pokerConfig.h"
#include "raylib.h"
#include "resources.h"
#include <stdio.h>
#include <string.h>

#define RENDER_BUFF_SIZE 2048
#define MAX_DISCARD 3

#define rem(x)                                                                 \
  if (x != NULL) {                                                             \
    free(x);                                                                   \
    x = NULL;                                                                  \
  }

char *renderbuff;

Rectangle centerbutton;
Rectangle leftbutton;
Rectangle rightbutton;
Rectangle cardArea;
int ready = 0;

void StartRender() {
  renderbuff = malloc(RENDER_BUFF_SIZE);

  centerbutton.width = 200;
  centerbutton.height = 50;
  centerbutton.x = (DEFAULT_WINDOW_WIDTH - centerbutton.width) / 2.0f;
  centerbutton.y = (float)(DEFAULT_WINDOW_HEIGHT - 220);

  leftbutton = centerbutton;
  leftbutton.x -= 210;

  rightbutton = centerbutton;
  rightbutton.x += 210;

  cardArea.width = DEFAULT_WINDOW_WIDTH - 600;
  cardArea.x = 300;
  cardArea.y = DEFAULT_WINDOW_HEIGHT - 160;
}
Vector2 playerpos[MAX_PLAYERS - 1] = {
    {.375f, 0.0f},    {.0097f, .350f}, {.7000f, 0.350f}, {0.6591f, .59f},
    {.0797f, .1205f}, {.0797f, .59},   {.6591f, 0.1205f}};

void DrawPlayers() {
  Game *game = GetGame();
  float scale = getScaler();
  Vector2 dimensions = {(float)GetRenderWidth(), (float)GetRenderHeight()};

  for (int i = 0; i < game->Players.length; i++) {
    Player *player = GetPlayerAt(i);
    Vector2 pos = playerpos[i];
    DrawText(player->name, (int)(pos.x * dimensions.x),
             (int)(pos.y * dimensions.y), (int)(TEXT_SIZE * scale), TEXT_COLOR);

    sprintf(renderbuff, "%lu", player->money);
    DrawText(renderbuff, (int)(pos.x * dimensions.x),
             (int)((pos.y * dimensions.y) + TEXT_SIZE * scale),
             (int)(TEXT_SIZE * scale), TEXT_COLOR);

    Texture cardback = GetCardBack();
    float scalec = (int)((334 * scale) / HAND_SIZE) / (float)cardback.width;
    for (int j = 0; j < HAND_SIZE; j++) {
      Texture tmp = cardback;
      if (GetShowingHands())
        tmp = GetCardTexture(player->cards[j].suit, player->cards[j].cardNum);
      DrawTextureEx(tmp,
                    (Vector2){pos.x * dimensions.x + j * CARD_WIDTH * scalec +
                                  j * 25 * scalec,
                              pos.y * dimensions.y + TEXT_SIZE * 2 * scale},
                    0, scalec, WHITE);
    }
  }
}
int discarding[MAX_DISCARD];
void NextRound() {
  ready = 0;
  memset(&discarding, -1, sizeof(discarding));
}
void UpdateRender() {}
void DrawReady() {
  Game *game = GetGame();
  float scale = getScaler();
  if (!ready && im_button(centerbutton, "Ready")) {
    Ready();
    ready = 1;
  }
  sprintf(renderbuff, "Players: %llu/8", game->Players.length + 1);
  DrawText(renderbuff, (int)((DEFAULT_WINDOW_WIDTH - 200) * scale), 10,
           (int)(TEXT_SIZE * scale), TEXT_COLOR);
}
void DrawCards() {
  Texture tmp = GetCardTexture(0, 0);
  Rectangle scaled = Scaled(cardArea);
  float screenscale = getScaler();
  float scale = (int)((scaled.width - 100) / HAND_SIZE) / (float)tmp.width;
  Hand *hand = CurrentHand();
  for (int i = 0; i < HAND_SIZE; i++) {
    DrawTextureEx(
        GetCardTexture(hand->cards[i].suit, hand->cards[i].cardNum),
        (Vector2){scaled.x + i * CARD_WIDTH * scale + i * 25, scaled.y}, 0,
        scale, WHITE);
    // Draw Highlight
    if (i < hand->length)
      DrawRectangleLinesEx(
          (Rectangle){scaled.x + i * CARD_WIDTH * scale + i * 25, scaled.y,
                      CARD_WIDTH * scale, CARD_HEIGHT * scale},
          CARD_HIGHLIGHT_THICKNESS, CARD_HIGHLIGHT);
    // Discard Logic
    if (getCurrentRound() == 1) {
      if (CheckCollisionPointRec(
              GetMousePosition(),
              (Rectangle){scaled.x + i * CARD_WIDTH * scale + i * 25, scaled.y,
                          CARD_WIDTH * scale, CARD_HEIGHT * scale}) &&
          IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

        int changed = 0;
        for (int j = 0; j < MAX_DISCARD; j++)
          if (discarding[j] == i) {
            discarding[j] = -1;
            changed = 1;
            break;
          }

        if (!changed)
          for (int j = 0; j < MAX_DISCARD; j++)
            if (discarding[j] == -1) {
              discarding[j] = i;
              break;
            }
      }
      // Draw Discard
      for (int j = 0; j < MAX_DISCARD; j++)
        if (discarding[j] == i)
          DrawLineEx(
              (Vector2){scaled.x + i * CARD_WIDTH * scale + i * 25, scaled.y},
              (Vector2){scaled.x + (i + 1) * CARD_WIDTH * scale + i * 25,
                        scaled.y + CARD_HEIGHT * scale},
              CARD_DISCARD_THICKNESS * screenscale, CARD_DISCARD);
    }
  }
}
int raiseAmount = 0;
void DrawBet() {
  DrawCards();
  if (!isTurn())
    return;
  char betbuff[256];
  sprintf(betbuff, "Current Bet: %d, Difference: %lu", GetMinBet(),
          GetMinBet() - GetLocalPlayer()->bet);
  im_label((Rectangle){centerbutton.x + 50, centerbutton.y - 65, 100, 30},
           betbuff);
  sprintf(betbuff, "%d", raiseAmount);
  im_label((Rectangle){centerbutton.x + 50, centerbutton.y - 35, 100, 30},
           betbuff);
  if (im_button((Rectangle){centerbutton.x, centerbutton.y - 35, 50, 30},
                "-")) {
    raiseAmount = (((raiseAmount - 10) > (0)) ? (raiseAmount - 10) : (0));
  }
  if (im_button((Rectangle){centerbutton.x + 150, centerbutton.y - 35, 50, 30},
                "+")) {
    raiseAmount += 10;
  }

  if (im_button(leftbutton, "Fold")) {
    Fold();
    raiseAmount = 0;
  }
  if (im_button(centerbutton, "Call")) {
    Call();
    raiseAmount = 0;
  }
  if (im_button(rightbutton, "Raise")) {
    Raise(raiseAmount);
    raiseAmount = 0;
  }
}
void DrawDiscard() {
  DrawCards();

  if (!ready && im_button(centerbutton, "Ready")) {
    DiscardHand(discarding[0], discarding[1], discarding[2]);

    memset(&discarding, -1, sizeof(discarding));
    ready = 1;
  }
}
void DrawRender() {
  DrawPlayers();
  switch (getCurrentRound()) {
  case -1:
    DrawReady();
    break;
  case 0:
  case 2:
    DrawBet();
    break;
  case 1:
    DrawDiscard();
    break;
  case 3:
    // draw results
    break;
  }
}
