#include "game.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Game game;

Game *create_game() {
  memset(&game, 0, sizeof(game));
  game.Players = Vector_Create(sizeof(Player));
  Vector_Reserve(&game.Players, MAX_PLAYERS);
  return &game;
}
int HasPlayer(unsigned long long player) {
  for (int i = 0; i < game.Players.length; i++) {
    if (player == GetPlayerAt(i)->id)
      return 1;
  }
  return 0;
}
int GetPlayerIndex(unsigned long long player) {
  for (int i = 0; i < game.Players.length; i++) {
    if (player == GetPlayerAt(i)->id)
      return i;
  }
  return -1;
}
Player *GetPlayer(unsigned long long player) {
  int i = GetPlayerIndex(player);
  if (i == -1)
    return NULL;
  return GetPlayerAt(i);
}
Player *GetPlayerAt(int player) {
  return ((Player *)Vector_At(&(game.Players), player));
}
Game *GetGame() {
  if (game.Players.length == 0) {
    if (game.deck != NULL)
      free(game.deck);
    game.deck = create_deck();
  }
  return &game;
}

void RemovePlayer(unsigned long long player) {
  Vector_RemoveAt(&(game.Players), GetPlayerIndex(player), 0);
}
Player *AddPlayer(unsigned long long player, char *name) {
  Player *newPlayer = GetPlayerAt(game.Players.length);
  memset(newPlayer, 0, sizeof(player));
  game.Players.length++;
  strncpy(newPlayer->name, name, NAME_LEN);
  newPlayer->name[NAME_LEN - 1] = '\0';
  newPlayer->id = player;
  return newPlayer;
}