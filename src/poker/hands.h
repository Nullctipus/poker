#ifndef HANDS_H
#define HANDS_H

#include "card.h"
#define HAND_SIZE 5
enum hand_type {
  HIGH_CARD = 0,
  ONE_PAIR = 1,
  TWO_PAIR = 2,
  THREE_OF_A_KIND = 3,
  STRAIGHT = 4,
  FLUSH = 5,
  FULL_HOUSE = 6,
  FOUR_OF_A_KIND = 7,
  STRAIGHT_FLUSH = 8,
};
typedef struct hand_struct {
  Card cards[HAND_SIZE];
  int length;
  enum hand_type type;
  int weight;
} Hand;
/*
 * @brief calculate how good a set of hands is
 * @param hand hand to calculate weight for
 */
void hand_weight(Hand *hand);
void swap_cards(Card *a, Card *b);
Hand create_hand(Card *cards);
const char *type_string(enum hand_type type);
int hand_compare(Hand *left, Hand *right);

#endif
