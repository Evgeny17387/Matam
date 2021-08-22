#ifndef _GAME_H
#define _GAME_H

#include "chessSystem.h"
#include "mtm_map/map.h"

#include <stdbool.h>

typedef struct game_t Game_t, *Game;

Game gameGetNext(Game game);

void gameFree(Game game);

Game gameCopy(Game game, Game next);

Game* gameGetNextAddress(Game game);

bool gameIsGameEqual(Game game, int first_player, int second_player);

Game gameCreate(int first_player, int second_player, Winner winner, int play_time);

void gameRemovePlayer(Game game, int player_id, Map players_global, Map players_local);

#endif //_GAME_H
