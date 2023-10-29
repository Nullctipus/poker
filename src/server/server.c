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

Vector Games;
void InitServer() { Games = Vector_Create(sizeof(Game)); }
void CreateNewGame(char *game) {
  Game r;
  memset(&r, 0, sizeof(r));
  strncpy_s(r.code, 8, game, 8);
  r.deck = create_deck();
  Vector_PushBack(&Games, &r);
}
int GetGame(char *game) {
  int currGame = -1;
  for (int i = 0; i < Games.length; i++) {
    if (strcmp(((Game *)Vector_At(&Games, i))->code, game) == 0) {
      currGame = i;
    }
  }
  if (currGame == -1) {
    CreateNewGame(game);
    currGame = Games.length - 1;
  }
  return currGame;
}
int HasPlayer(Game *game, unsigned long long player) {
  for (int i = 0; i < game->playerCount; i++) {
    if (player == game->players[i].id)
      return 1;
  }
  return 0;
}
Player *GetPlayer(Game *game, unsigned long long player) {
  for (int i = 0; i < game->playerCount; i++) {
    if (player == game->players[i].id)
      return game->players + i;
  }
  return NULL;
}
char buff[2048];
void ProcessIncoming(unsigned long long socket, char *game, char *type,
                     char *value) {
  int gameIndex = GetGame(game);
  Game *currGame = (Game *)Vector_At(&Games, gameIndex);
  if (!HasPlayer(currGame, socket)) {
    if (type != NULL && strcmp("join", type) == 0 && value != NULL &&
        currGame->playerCount < MAX_PLAYERS) {

      sprintf_s(
          buff, 2048,
          "join;%d,%s,%llu,%s,%llu,%s,%llu,%s,%llu,%s,%llu,%s,%llu,%s,%llu",
          currGame->playerCount, currGame->players[0].name,
          currGame->players[0].id, currGame->players[1].name,
          currGame->players[1].id, currGame->players[2].name,
          currGame->players[2].id, currGame->players[3].name,
          currGame->players[3].id, currGame->players[4].name,
          currGame->players[4].id, currGame->players[5].name,
          currGame->players[5].id, currGame->players[6].name,
          currGame->players[6].id);
      sendSingle(socket, buff);
      Player *newPlayer = currGame->players + currGame->playerCount++;
      strncpy_s(newPlayer->name, NAME_LEN, value, NAME_LEN);
      newPlayer->id = socket;
      sprintf_s(buff, 2048, "joined;%s,%llu", newPlayer->name, socket);
      sendAll(buff, socket);
      return;
    } else {
      if (currGame->playerCount == 0) {
        free(currGame->deck);
        Vector_RemoveAt(&Games, gameIndex, 0);
      }
      sendSingle(socket, "full");
      return;
    }
  }
  // Disconnect
  // Set Bet
  // Request Hand
  // Show Hand
  // Replace Hand
}
