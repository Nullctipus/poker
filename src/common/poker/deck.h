#pragma once

#include "card.h"

#define MAX_DECK_LEN 52

typedef struct deck_struct {
  Card cards[MAX_DECK_LEN];
  int length;
} Deck;

Deck *create_deck();

void remove_from_deck(Deck *deck, int index);
Card deck_nextCard(Deck *deck);
void reset_deck(Deck *deck);