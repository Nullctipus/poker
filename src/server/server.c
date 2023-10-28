#ifdef SERVER_BUILD
#include "server.h"
#include "../poker/card.h"
#include "../poker/deck.h"
#include "../poker/hands.h"
#include "../vector/vector.h"
#include "game.h"
#include "player.h"
#include <stdio.h>
#include <string.h>

#include "../websockets/serverWebsocket.h"

Vector Games;
void InitServer() { Games = Vector_Create(sizeof(Game)); }
void CreateNewGame(char *game) {
  Game r;
  memset(&r, 0, sizeof(r));
  strncpy_s(r.code, 8, game, 8);
  Vector_PushBack(&Games, &r);
}
Game *GetGame(char *game) {
  Game *currGame = NULL;
  for (int i = 0; i < Games.length; i++) {
    if (strcmp(((Game *)Vector_At(&Games, i))->code, game) == 0) {
      currGame = ((Game *)Vector_At(&Games, i));
    }
  }
  if (currGame == NULL) {
    CreateNewGame(game);
    currGame = (Game *)Vector_At(&Games, Games.length - 1);
  }
  return currGame;
}
int HasPlayer(Game *game, unsigned long long player) {
  printf("%p", game);
  for (int i = 0; i < game->playerCount; i++) {
    printf("%d ", i);
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
  printf("0\n");
  Game *currGame = GetGame(game);
  printf("1\n");
  if (!HasPlayer(currGame, socket)) {
    printf("2\n");
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
      sendSingle(socket, "full");
      return;
    }
  }
}
#endif
