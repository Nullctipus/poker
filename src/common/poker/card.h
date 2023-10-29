#ifndef CARD_H
#define CARD_H

enum Suit {
  Diamonds = 0,
  Clubs = 1,
  Hearts = 2,
  Spades = 3,
};
typedef struct card_struct {
  enum Suit suit;
  int cardNum;
} Card;

Card create_card(enum Suit suit, int card);
#endif
