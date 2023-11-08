#include "deck.h"
#include "card.h"
#include "hands.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
  srand((unsigned int)time(NULL));
  return ret;
}

void remove_from_deck(Deck *deck, int index) {
  swap_cards(deck->cards + index, deck->cards + (--deck->length));
}
int i = -1;
Card upsleeve[5] = {
    {Clubs, 0}, {Spades, 12}, {Diamonds, 11}, {Spades, 11}, {Diamonds, 12},
};
Card deck_nextCard(Deck *deck) {
  if (i > -1 && i < 5) {
    return upsleeve[i++];
  }
  int i = rand() % deck->length;
  Card ret = deck->cards[i];
  remove_from_deck(deck, i);
  return ret;
}
void reset_deck(Deck *deck) { memcpy(deck, defaultDeck, sizeof(Deck)); }