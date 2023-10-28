#include "card.h"
Card create_card(enum Suit suit, int card) {
  Card ret;
  ret.suit = suit;
  ret.cardNum = card;
  return ret;
}
