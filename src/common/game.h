#pragma once

#include "player.h"
#include "poker/deck.h"
#include "../common/vector/vector.h"
#define MAX_PLAYERS 8

typedef struct game_struct {
  Vector Players;
  int dealer;
  Deck *deck;
} Game;

Game *create_game();
int HasPlayer(unsigned long long player) ;
int GetPlayerIndex( unsigned long long player);
Player *GetPlayer(unsigned long long player);
Player *GetPlayerAt(int player);
void RemovePlayer(unsigned long long player);
Player *AddPlayer(unsigned long long player, char* name);
Game *GetGame();