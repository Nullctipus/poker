#pragma once

#define NAME_LEN 64
#include "poker/card.h"
#include "poker/hands.h"

typedef struct player_struct {
  char name[NAME_LEN];
  unsigned long long id;
  unsigned long bet;
  unsigned long money;
  int folded;
  int dealer;
  Card cards[HAND_SIZE];
} Player;

