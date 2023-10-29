#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "poker/deck.h"
#define MAX_PLAYERS 8

typedef struct game_struct {
  char code[8];
  Player players[MAX_PLAYERS];
  int playerCount;
  int dealer;
  Deck *deck;
} Game;

#endif
