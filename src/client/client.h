#pragma once

#include "../common/poker/hands.h"
#include "../common/player.h"

void StartClient();
void ProcessData(char* type, char* value);
void DiscardHand(int c1,int c2,int c3);
void Ready();
void Call();
void Raise(int amount);
void Fold();
int isTurn();
int getCurrentRound();
Hand *CurrentHand();
Player *GetLocalPlayer();
int GetMinBet();