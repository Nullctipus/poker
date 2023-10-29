#include "hands.h"
#include "card.h"
#include <string.h>

#define KINDS_LEN 13
int kinds[KINDS_LEN];
const char *hand_strings[] = {
    "High card", "One pair",   "Two pair",       "Three of a kind", "Straight",
    "Flush",     "Full House", "Four of a kind", "Straight flush",
};
const char *type_string(enum hand_type type) { return hand_strings[type]; }
void swap_cards(Card *a, Card *b) {
  Card tmp;
  tmp.cardNum = a->cardNum;
  tmp.suit = a->suit;
  a->cardNum = b->cardNum;
  a->suit = b->suit;
  b->cardNum = tmp.cardNum;
  b->suit = tmp.suit;
}
void setFirst(Card *cards, int type, int offset, int count) {
  for (int i = offset; i < count + offset; i++) {
    if (cards[i].cardNum == type)
      continue;
    for (int j = i + 1; j < HAND_SIZE; j++) {
      if (cards[j].cardNum == type) {
        swap_cards(cards + i, cards + j);
        break;
      }
    }
  }
}

void setType(Hand *hand) {
  int flush = 1, straight = 1, i, n;
  enum Suit s = hand->cards[0].suit;
  for (i = 1; i < HAND_SIZE; i++)
    if (s != hand->cards[i].suit) {
      flush = 0;
      break;
    }
  n = hand->cards[0].cardNum;
  for (i = 1; i < HAND_SIZE; i++)
    if (n + i != hand->cards[i].cardNum) {
      straight = 0;
      break;
    }

  if (straight && flush) {
    hand->length = HAND_SIZE;
    hand->type = STRAIGHT_FLUSH;
    return;
  }
  if (flush) {
    hand->length = HAND_SIZE;
    hand->type = FLUSH;
    return;
  }
  if (straight) {
    hand->length = HAND_SIZE;
    hand->type = STRAIGHT;
    return;
  }
  memset(kinds, 0, KINDS_LEN * sizeof(int));
  for (i = 0; i < HAND_SIZE; i++)
    kinds[hand->cards[i].cardNum]++;

  int largest = 0, slargest = 0;
  for (i = 0; i < KINDS_LEN; i++) {
    if (kinds[i] > kinds[largest]) {
      largest = i;
      continue;
    }
    if (kinds[i] > kinds[slargest])
      slargest = i;
  }
  if (kinds[largest] == 4) {
    hand->length = 4;
    if (hand->cards[0].cardNum != i)
      swap_cards(hand->cards, hand->cards + 4);
    hand->type = FOUR_OF_A_KIND;
    return;
  }
  if (kinds[largest] == 3) {
    setFirst(hand->cards, largest, 0, 3);

    if (kinds[slargest] == 2) {
      hand->type = FULL_HOUSE;
      hand->length = 5;
      return;
    }
    hand->type = FOUR_OF_A_KIND;
    hand->length = 4;
    return;
  }
  if (kinds[largest] == 2) {
    setFirst(hand->cards, largest, 0, 2);
    if (kinds[slargest] == 2) {
      setFirst(hand->cards, slargest, 2, 2);
      hand->type = TWO_PAIR;
      hand->length = 4;
      return;
    }
    hand->type = ONE_PAIR;
    hand->length = 2;
    return;
  }
  hand->type = HIGH_CARD;
  hand->length = 1;
  swap_cards(hand->cards, hand->cards + 4); // set largest to first card
}
void sort_cards(Card *cards) {
  int i, j, min_idx;
  for (i = 0; i < HAND_SIZE - 1; i++) {
    min_idx = i;

    for (j = i + 1; j < HAND_SIZE; j++)
      if (cards[j].cardNum < cards[min_idx].cardNum)
        min_idx = j;

    if (min_idx != i)
      swap_cards(cards + i, cards + min_idx);
  }
}
void hand_weight(Hand *hand) {
  setType(hand);
  hand->weight = 0;
  for (int i = 0; i < HAND_SIZE; i++) {
    hand->weight += hand->cards[i].cardNum * (1 + (i < hand->length));
  }
}

Hand create_hand(Card *cards) {
  Hand ret;
  memcpy(ret.cards, cards, HAND_SIZE * sizeof(Card));
  sort_cards(ret.cards);
  hand_weight(&ret);
  return ret;
}

int hand_compare(Hand *left, Hand *right) {
  if (left->type == right->type)
    return left->weight < right->weight;
  return left->type < right->type;
}
