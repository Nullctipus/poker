#include "server.h"
#include "../common/game.h"
#include "../common/player.h"
#include "../common/poker/card.h"
#include "../common/poker/deck.h"
#include "../common/poker/hands.h"
#include "../common/vector/vector.h"
#include <stdio.h>
#include <string.h>

#include "serverWebsocket.h"

void InitServer() { create_game(); }

char buff[2048];
unsigned int ready = 0;

void ProcessDisconnect(unsigned long long socket) {
  GetGame();
  if (!HasPlayer(socket))
    return;

  sprintf(buff, "leave;%llu\n", socket);

  sendAll(buff, socket);
}

void HandleJoin(Game *game, unsigned long long socket, char *value) {
  sprintf(buff, "join;%llu\n", socket);
  sendSingle(socket, buff);
  sprintf(
      buff,
      "players;%llu,%s,%llu,%s,%llu,%s,%llu,%s,%llu,%s,%llu,%s,%llu,%s,%llu\n",
      game->Players.length, GetPlayerAt(0)->name, GetPlayerAt(0)->id,
      GetPlayerAt(1)->name, GetPlayerAt(1)->id, GetPlayerAt(2)->name,
      GetPlayerAt(2)->id, GetPlayerAt(3)->name, GetPlayerAt(3)->id,
      GetPlayerAt(4)->name, GetPlayerAt(4)->id, GetPlayerAt(5)->name,
      GetPlayerAt(5)->id, GetPlayerAt(6)->name, GetPlayerAt(6)->id);
  buff[2047] = '\0';
  sendSingle(socket, buff);
  Player *newPlayer = AddPlayer(socket, value);
  sprintf(buff, "joined;%s,%llu\n", newPlayer->name, socket);
  buff[2047] = '\0';
  sendAll(buff, socket);
}
void HandleReady(Game *game, unsigned long long socket) {
  ready |= 1 << GetPlayerIndex(socket);
  unsigned int allReady = UINT_MAX >> (32u - game->Players.length);
  ready &= allReady;

  if (allReady == ready) {
    ready = 0;
    GetPlayerAt(0)->dealer = 1;
    sprintf(buff, "start;%llu", GetPlayerAt(0)->id);
    sendAll(buff, 0);
  }
}

void ProcessIncoming(unsigned long long socket, char *type, char *value) {
  Game *game = GetGame();
  // Join
  if (!HasPlayer(socket)) {
    if (type != NULL && strcmp("join", type) == 0 && value != NULL &&
        game->Players.length < MAX_PLAYERS) {
      HandleJoin(game, socket, value);
    } else {
      sendSingle(socket, "full\n");
    }
    return;
  }

  // Ready
  if (strcmp("ready", type)) {
    HandleReady(game, socket);
  }
  // Raise
  if (strcmp("raise", type) == 0 && value != NULL) {
  }
  // Call
  // Fold
  // Request Hand
  // Show Hand
  // Replace Hand
}
