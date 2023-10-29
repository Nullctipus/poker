#include "deck.h"
#define CARD_H
#include "card.h"
#include "hands.h"
#include <stdlib.h>
#include <string.h>

Deck *defaultDeck = NULL;

Deck *create_deck() {
  if (defaultDeck == NULL) {
    defaultDeck = malloc(sizeof(Deck));
    for (int suit = 0; suit < 4; suit++)
      for (int card = 0; card < 13; card++)
        defaultDeck->cards[suit * 13 + card] = create_card(suit, card);
    defaultDeck->length = MAX_DECK_LEN;
  }
  Deck *ret = malloc(sizeof(Deck));
  memcpy(ret, defaultDeck, sizeof(Deck));
  return ret;
}

void remove_from_deck(Deck *deck, int index) {
  swap_cards(deck->cards + index, deck->cards + (--deck->length));
}
