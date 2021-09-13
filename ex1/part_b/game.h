#ifndef _GAME_H
#define _GAME_H

#include "chessSystem.h"
#include "map.h"

#include <stdbool.h>

typedef struct game_t Game_t, *Game;

int gameGetFirstPlayer(Game game);
int gameGetSecondPlayer(Game game);
Winner gameGetWinner(Game game);
Game gameGetNext(Game game);

Game* gameGetNextAddress(Game game);

void gameFree(Game game);
Game gameCopy(Game game, Game next);

bool gameIsGameEqual(Game game, int first_player, int second_player);

Game gameCreate(int first_player, int second_player, Winner winner, int play_time);

void gameRemovePlayer(Game game, int player_id);

bool gameIsPlayerLeft(Game game);

#endif //_GAME_H
