#include "chessSystem.h"

#include <stdlib.h>

// ToDo: check if should be 0 or 1
#define MIN_GAMES_PER_PLAYER 1

struct chess_system_t {
    
    int size;
};

ChessSystem chessCreate()
{
    ChessSystem chess_system = malloc(sizeof(*chess_system));
    
    return chess_system;
}

void chessDestroy(ChessSystem chess)
{
    free(chess);
}

ChessResult chessAddTournament(
    ChessSystem chess,
    int tournament_id,
    int max_games_per_player,
    const char* tournament_location)
{
    if ((NULL == chess) || (NULL == tournament_location))
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (max_games_per_player < MIN_GAMES_PER_PLAYER)
    {
        return CHESS_INVALID_MAX_GAMES;
    }

    return CHESS_SUCCESS;
}
