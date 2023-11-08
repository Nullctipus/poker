#include "client.h"
#include "../common/game.h"
#include "../common/player.h"
#include "../common/vector/vector.h"
#include "clientWebsocket.h"
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

Player localPlayer;
Hand currentHand;
int myTurn = 0;
#define BUFF_SIZE 2048
char *clientBuff;
int currentRound = -1;
int minBet = 0;

void StartClient() {
  memset(&localPlayer, 0, sizeof(localPlayer));
  create_game();
  clientBuff = malloc(BUFF_SIZE);
}
void ProcessJoin(char *value) {
  char *name, *id;
  name = value;
  id = strchr(name, ',');
  if (NULL != id) {
    *id = 0;
    id++;
  }
  unsigned long long ullid;
  sscanf(id, "%llu", &ullid);
  AddPlayer(ullid, name);
}
void ProcessLeave(char *value) {
  unsigned long long uid;
  sscanf(value, "%llu", &uid);
  RemovePlayer(uid);
}
void ProcessPlayers(char *value) {
  Game *game = GetGame();
  sscanf(
      value,
      "%llu,%[^,],%llu,%[^,],%llu,%[^,],%llu,%[^,],%llu,%[^,],%llu,%[^,],%llu,"
      "%[^,],%llu",
      &game->Players.length, GetPlayerAt(0)->name, &GetPlayerAt(0)->id,
      GetPlayerAt(1)->name, &GetPlayerAt(1)->id, GetPlayerAt(2)->name,
      &GetPlayerAt(2)->id, GetPlayerAt(3)->name, &GetPlayerAt(3)->id,
      GetPlayerAt(4)->name, &GetPlayerAt(4)->id, GetPlayerAt(5)->name,
      &GetPlayerAt(5)->id, GetPlayerAt(6)->name, &GetPlayerAt(6)->id);
  for (int i = 0; i < game->Players.length; i++) {
    GetPlayerAt(i)->money = 1000;
  }
}
void NextTurn(char *value) {
  myTurn = *(unsigned long long *)value == localPlayer.id;
}
void ProcessRound(char *value) {
  unsigned long long nextPlayer;
  sscanf(value, "%d,%llu", &currentRound, &nextPlayer);
  myTurn = nextPlayer == localPlayer.id;
  NextRound();
}
void ProcessHand(char *value) {
  memcpy(&currentHand, value, sizeof(Hand));
  printf("Hand: %s (%d,%d),(%d,%d),(%d,%d),(%d,%d),(%d,%d)\n",
         type_string(currentHand.type), currentHand.cards[0].suit,
         currentHand.cards[0].cardNum, currentHand.cards[1].suit,
         currentHand.cards[1].cardNum, currentHand.cards[2].suit,
         currentHand.cards[2].cardNum, currentHand.cards[3].suit,
         currentHand.cards[3].cardNum, currentHand.cards[4].suit,
         currentHand.cards[4].cardNum);
}
void ProcessBet(char *value) {
  unsigned long long player;
  unsigned long bet;
  sscanf(value, "%llu,%lu", &player, &bet);
  if (player == localPlayer.id)
    localPlayer.bet = bet;
  else
    GetPlayer(player)->bet = bet;
  minBet = bet;
}
void ProcessData(char *type, char *value) {
  if (strcmp(type, "leave") == 0) {
    ProcessLeave(value);
  }
  if (strcmp(type, "join") == 0) {
    sscanf(value, "%llu", &localPlayer.id);
  }
  if (strcmp(type, "joined") == 0) {
    ProcessJoin(value);
    currentRound = -1;
    NextRound();
  }
  if (strcmp(type, "players") == 0) {
    ProcessPlayers(value);
  }
  if (strcmp(type, "hand") == 0) {
    ProcessHand(type + 7);
  }
  if (strcmp(type, "bet") == 0) {
    ProcessBet(value);
  }
  if (strcmp(type, "start") == 0) {
    currentRound = 0;
    NextRound();
    NextTurn(type + 8);
  }
  if (strcmp(type, "next") == 0) {
    NextTurn(type + 6);
  }
  if (strcmp(type, "round") == 0) {
    ProcessRound(value);
  }
}

void DiscardHand(int c1, int c2, int c3) {
  sprintf(clientBuff, "discard;%d,%d,%d", c1, c2, c3);
  websocketSend(clientBuff);
}
void Ready() { websocketSend("ready"); }
void Call() { websocketSend("call"); }
void Raise(int amount) {
  sprintf(clientBuff, "raise;%d", amount);
  websocketSend(clientBuff);
}
void Fold() { websocketSend("fold"); }

int getCurrentRound() { return currentRound; }
int isTurn() { return myTurn; }
Hand *CurrentHand() { return &currentHand; }
Player *GetLocalPlayer() { return &localPlayer; }
int GetMinBet() { return minBet; }