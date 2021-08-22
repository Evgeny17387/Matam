#ifndef _TOURNMENT_H
#define _TOURNMENT_H

#include "chessSystem.h"
#include "mtm_map/map.h"

#include <stdbool.h>

typedef struct tournament_t Tournament_t, *Tournament;

MapDataElement tournamentCopy(MapDataElement n);

void tournamentFree(MapDataElement n);

Tournament tournamentCreate(int max_games_per_player, const char* tournament_location);

bool tournamentIsGameExists(Tournament tournament, int first_player, int second_player);

bool tournamentCheckLocationValidity(const char* tournament_location);

bool tournamentIsPlayersExceededMaxGames(Tournament tournament, int player_id);

bool tournamentIsEnded(Tournament tournament);

bool tournamentAddGame(Tournament tournament, int first_player, int second_player, Winner winner, int play_time);

void tournamentUpdateStatisticsOnNewGame(Tournament tournament, int play_time);

void tournamentIncreaseNumberOfPlayers(Tournament tournament);

Map tournamentGetPlayers(Tournament tournament);

void tournamentSubtractFromGlobalsPlayersStatistics(Tournament tournament, Map players_global);

void tournamentRemovePlayer(Tournament tournament, int player_id, Map players_global);

void tournamentPrintStatistics(Tournament tournament, FILE* file);

int tournamentGetNumberOfGames(Tournament tournament);

void tournamentEnd(Tournament tournament);

#endif //_TOURNMENT_H
