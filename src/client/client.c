#include "client.h"
#include "../common/game.h"
#include "../common/player.h"
#include "../common/vector/vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "raylib.h"

Player localPlayer;

void StartClient() {
  memset(&localPlayer, 0, sizeof(localPlayer));
  create_game();
}
void ProcessJoin(char *value) {
  char *name, *id;
  name = value;
  id = strchr(name, ',');
  if (NULL != id) {
    *id = 0;
    id++;
  }
  unsigned long long ullid;
  sscanf(id, "%llu", &ullid);
  AddPlayer(ullid, name);
}
void ProcessLeave(char *value) {
  unsigned long long uid;
  sscanf(value, "%llu", &uid);
  RemovePlayer(uid);
}
void ProcessPlayers(char *value) {
  Game *game = GetGame();
  scanf(value,
        "%d,%[^,],%llu,%[^,],%llu,%[^,],%llu,%[^,],%llu,%[^,],%llu,%[^,],%llu,%"
        "[^,],%llu",
        game->Players.length, GetPlayerAt(0)->name, &GetPlayerAt(0)->id,
        GetPlayerAt(1)->name, &GetPlayerAt(1)->id, GetPlayerAt(2)->name,
        &GetPlayerAt(2)->id, GetPlayerAt(3)->name, &GetPlayerAt(3)->id,
        GetPlayerAt(4)->name, &GetPlayerAt(4)->id, GetPlayerAt(5)->name,
        &GetPlayerAt(5)->id, GetPlayerAt(6)->name, &GetPlayerAt(6)->id);
}
void ProcessData(char *type, char *value) {
  if (strcmp(type, "leave") == 0) {
    ProcessLeave(value);
  }
  if (strcmp(type, "join") == 0) {
    sscanf(value, "%llu", &localPlayer.id);
  }
  if (strcmp(type, "joined") == 0) {
    ProcessJoin(value);
  }
  if (strcmp(type, "players") == 0) {
    ProcessPlayers(value);
  }
}