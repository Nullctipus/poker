#ifndef PLAYER_H
#define PLAYER_H

#define NAME_LEN 64
#include "poker/card.h"
#include "poker/hands.h"

typedef struct player_struct {
  char name[NAME_LEN];
  unsigned long long id;
  unsigned long bet;
  Card cards[HAND_SIZE];
} Player;

#endif
