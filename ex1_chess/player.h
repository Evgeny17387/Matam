#ifndef _PLAYER_H
#define _PLAYER_H

#include "map.h"

#include <stdio.h>
#include <stdbool.h>

typedef struct player_t Player_t, *Player;

int playerGetWins(Player player);
int playerGetLosses(Player player);
int playerGetDraws(Player player);
int playerGetGames(Player player);

void playerIncreaseWins(Player player);
void playerDecreaseLosses(Player player);
void playerDecreaseDraws(Player player);

MapDataElement playerCopy(MapDataElement n);

void playerFree(MapDataElement n);

bool playerAddOrUpdate(Map players, int player_id, bool is_win, bool is_losse, bool is_draw, int play_time);

void playerSubtract(Player player_global, Player player_local);

bool playerPrintPlayersLevelIdSorted(Map players, FILE* file);

double playerGetAveragePlayTime(Player player);

#endif //_PLAYER_H
