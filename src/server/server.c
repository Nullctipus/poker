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


void InitServer() { }

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
Game game = {0};
Game GetGame(){
  if(game.playerCount == 0){
    if(game.deck != NULL)
      free(game.deck);
    game.deck = create_deck();
  }
  return game;
}
char buff[2048];

void ProcessDisconnect(unsigned long long socket)
{
  Game currGame = GetGame();
  if(!HasPlayer(&currGame,socket)) return;
  sprintf(buff,"leave;%llu",socket);

  sendAll(buff,socket);
}
void ProcessIncoming(unsigned long long socket, char *type,
                     char *value) {
  Game currGame = GetGame();
  if (!HasPlayer(&currGame, socket)) {
    if (type != NULL && strcmp("join", type) == 0 && value != NULL &&
        currGame.playerCount < MAX_PLAYERS) {
        sprintf(buff,"join;%llu",socket);
        sendSingle(socket, buff);
      sprintf_s(
          buff, 2048,
          "players;%d,%s,%llu,%s,%llu,%s,%llu,%s,%llu,%s,%llu,%s,%llu,%s,%llu",
          currGame.playerCount, currGame.players[0].name,
          currGame.players[0].id, currGame.players[1].name,
          currGame.players[1].id, currGame.players[2].name,
          currGame.players[2].id, currGame.players[3].name,
          currGame.players[3].id, currGame.players[4].name,
          currGame.players[4].id, currGame.players[5].name,
          currGame.players[5].id, currGame.players[6].name,
          currGame.players[6].id);
      sendSingle(socket, buff);
      Player *newPlayer = currGame.players + currGame.playerCount++;
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
  // Set Bet
  // Request Hand
  // Show Hand
  // Replace Hand
}
