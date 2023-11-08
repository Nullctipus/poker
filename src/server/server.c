#include "server.h"
#include "../common/game.h"
#include "../common/player.h"
#include "../common/poker/card.h"
#include "../common/poker/deck.h"
#include "../common/poker/hands.h"
#include "../common/vector/vector.h"
#include <stdio.h>
#include <string.h>

#include "serverWebsocket.h"

void InitServer() { create_game(); }

void SendHand(Game *game, unsigned long long socket);
char clientBuff[2048];
unsigned int ready = 0;
unsigned int turn = 0;
unsigned int lastRaise = 0;
unsigned long potPerPlayer = 0;
int currentRound = 0;

#define DISCARD_ROUND 1
#define BETTING_ROUND_1 0
#define BETTING_ROUND_2 2
#define REVEAL_ROUND 3

void NextGame() {
  Game *game = GetGame();
  reset_deck(game->deck);
  currentRound = 0;
  turn = 0;
  Player *dealer;
  for (int i = 0; i < game->Players.length; i++) {
    dealer = GetPlayerAt(i);
    if (dealer->dealer) {
      dealer->dealer = 0;
      dealer = GetPlayerAt((i + 1) % game->Players.length);
      dealer->dealer = 1;
      break;
    }
  }
  sprintf(clientBuff, "start;%llu", dealer->id);
  sendAll(clientBuff, 0);
  for (int i = 0; i < game->Players.length; i++) {
    SendHand(game, GetPlayerAt(i)->id);
  }
}
void HandleSettled(Game *game, unsigned long long next) {
  currentRound++;
  if (currentRound == 4) {
    NextGame();
    return;
  }

  sprintf(clientBuff, "round;%d;%llu", currentRound, next);
  sendAll(clientBuff, 0);
  switch (currentRound) {
  case DISCARD_ROUND:
    // allow discarding cards
    break;
  case BETTING_ROUND_2:
    // allow second bet
    break;
  case REVEAL_ROUND:
    // reveal cards
    // TODO send cards to every player
    // TODO Decrease bet amount
    // TODO wait 5 seconds then next game
    break;
  }
}
void NextTurn(Game *game, int forceRound) {
  Player *player;
  do {
    turn = (turn + 1) % game->Players.length;
    player = GetPlayerAt(turn);
  } while (player->folded);

  if (turn == lastRaise && !forceRound)
    HandleSettled(game, player->id);

  strcpy(clientBuff, "next;");
  memcpy(clientBuff + 6, &player->id, sizeof(unsigned long long));
  sendAllBin(clientBuff, 0, 6 + sizeof(unsigned long long));
}

void ProcessDisconnect(unsigned long long socket) {
  GetGame();
  if (!HasPlayer(socket))
    return;
  RemovePlayer(socket);
}

void HandleJoin(Game *game, unsigned long long socket, char *value) {
  sprintf(clientBuff, "join;%llu\n", socket);
  sendSingle(socket, clientBuff);
  sprintf(
      clientBuff,
      "players;%llu,%s,%llu,%s,%llu,%s,%llu,%s,%llu,%s,%llu,%s,%llu,%s,%llu\n",
      game->Players.length, GetPlayerAt(0)->name, GetPlayerAt(0)->id,
      GetPlayerAt(1)->name, GetPlayerAt(1)->id, GetPlayerAt(2)->name,
      GetPlayerAt(2)->id, GetPlayerAt(3)->name, GetPlayerAt(3)->id,
      GetPlayerAt(4)->name, GetPlayerAt(4)->id, GetPlayerAt(5)->name,
      GetPlayerAt(5)->id, GetPlayerAt(6)->name, GetPlayerAt(6)->id);

  clientBuff[2047] = '\0';
  sendSingle(socket, clientBuff);
  if (strcmp("", value) == 0)
    strcpy(value, "no name");

  Player *newPlayer = AddPlayer(socket, value);
  sprintf(clientBuff, "joined;%s,%llu\n", newPlayer->name, socket);
  clientBuff[2047] = '\0';
  sendAll(clientBuff, socket);
}

void SendHand(Game *game, unsigned long long socket) {
  Player *player = GetPlayer(socket);
  for (int i = 0; i < HAND_SIZE; i++) {
    if (player->cards[i].suit != -1)
      continue;
    player->cards[i] = deck_nextCard(game->deck);
  }
  Hand hand = create_hand(player->cards);
  memcpy(player->cards, hand.cards, sizeof(player->cards));
  sprintf(clientBuff, "hand;");
  memcpy(clientBuff + 7, &hand, sizeof(Hand));
  sendSingleBin(socket, clientBuff, strlen(clientBuff) + sizeof(hand));
}

void HandleReady(Game *game, unsigned long long socket) {
  ready |= 1 << GetPlayerIndex(socket);
  unsigned int allReady = UINT_MAX >> (32u - game->Players.length);
  ready &= allReady;
  if (allReady != ready)
    return;
  ready = 0;
  currentRound = 0;
  turn = 0;
  GetPlayerAt(0)->dealer = 1;
  memset(clientBuff, 0, 128);
  strcpy(clientBuff, "start;");
  memcpy(clientBuff + 8, &GetPlayerAt(0)->id, sizeof(unsigned long long));
  sendAllBin(clientBuff, 0, 8 + sizeof(unsigned long long));
  for (int i = 0; i < game->Players.length; i++) {
    SendHand(game, GetPlayerAt(i)->id);
  }
}
void HandleRaise(Game *game, unsigned long long socket, char *value) {
  int ivalue = atoi(value);
  int playerIndex = GetPlayerIndex(socket);
  if (playerIndex != turn)
    return;

  Player *player = Vector_At(&game->Players, playerIndex);
  player->bet = min(potPerPlayer + ivalue, player->money);
  potPerPlayer = player->bet;
  sprintf(clientBuff, "bet;%llu,%lu", socket, player->bet);
  sendAll(clientBuff, 0);
  lastRaise = playerIndex;
  NextTurn(game, 1);
}
void HandleCall(Game *game, unsigned long long socket) {
  int playerIndex = GetPlayerIndex(socket);
  if (playerIndex != turn)
    return;

  Player *player = Vector_At(&game->Players, playerIndex);
  player->bet = potPerPlayer;
  sprintf(clientBuff, "bet;%llu,%lu", socket, player->bet);
  sendAll(clientBuff, 0);

  NextTurn(game, 0);
}
void HandleFold(Game *game, unsigned long long socket) {
  int playerIndex = GetPlayerIndex(socket);
  printf("\n%d == %d\n", playerIndex, turn);
  if (playerIndex != turn)
    return;

  Player *player = Vector_At(&game->Players, playerIndex);
  player->folded = 1;
  sprintf(clientBuff, "fold;%llu", socket);
  sendAll(clientBuff, 0);
  NextTurn(game, 0);
}
void DiscardCards(Game *game, unsigned long long socket, int c1, int c2,
                  int c3) {
  int playerIndex = GetPlayerIndex(socket);
  if (currentRound != DISCARD_ROUND)
    return;
  Player *player = GetPlayer(socket);
  int c = 0;
  if (c1 > -1) {
    c++;
    player->cards[c1].suit = -1;
  }
  if (c2 > -1) {
    c++;
    player->cards[c2].suit = -1;
  }
  if (c3 > -1) {
    c++;
    player->cards[c3].suit = -1;
  }
  sprintf(clientBuff, "discard;%llu,%d", socket, c);
  sendAll(clientBuff, socket);

  SendHand(game, socket);

  NextTurn(game, 0);
}

void ProcessIncoming(unsigned long long socket, char *type, char *value) {
  Game *game = GetGame();
  // Join
  if (!HasPlayer(socket)) {
    if (type != NULL && strcmp("join", type) == 0 && value != NULL &&
        game->Players.length < MAX_PLAYERS) {
      HandleJoin(game, socket, value);
    } else {
      sendSingle(socket, "full\n");
    }
    return;
  }

  // Ready
  if (strcmp("ready", type) == 0) {
    HandleReady(game, socket);
  }
  // Raise
  if (strcmp("raise", type) == 0 && value != NULL) {
    HandleRaise(game, socket, value);
  }
  // Call
  if (strcmp("call", type) == 0) {
    HandleCall(game, socket);
  }
  // Fold
  if (strcmp("fold", type) == 0) {
    HandleFold(game, socket);
  }
  // Request Hand
  if (strcmp("hand", type) == 0) {
    SendHand(game, socket);
  }
  // Replace Hand
  if (strcmp("discard", type) == 0 && value != NULL) {
    int c[3];
    sscanf(value, "%d,%d,%d", c, c + 1, c + 2);
    DiscardCards(game, socket, c[0], c[1], c[2]);
  }
}
