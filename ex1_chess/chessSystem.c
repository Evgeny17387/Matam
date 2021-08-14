#include "chessSystem.h"
#include "mtm_map/map.h"

#include <stdlib.h>

// ToDo: check if should be 0 or 1
#define MIN_GAMES_PER_PLAYER 1

typedef struct tournament_t {
    int max_games_per_player;
} *Tournament;

struct chess_system_t {
    Map tournaments;
};

static MapKeyElement copyKeyInt(MapKeyElement n)
{
    if (!n)
    {
        return NULL;
    }

    int *copy = malloc(sizeof(*copy));
    if (!copy)
    {
        return NULL;
    }

    *copy = *(int *) n;

    return copy;
}

static MapDataElement copyDataChar(MapDataElement n)
{
    if (!n)
    {
        return NULL;
    }

    char *copy = malloc(sizeof(*copy));
    if (!copy)
    {
        return NULL;
    }

    *copy = *(char *) n;

    return (MapDataElement) copy;
}

static void freeInt(MapKeyElement n)
{
    free(n);
}

static void freeChar(MapDataElement n)
{
    free(n);
}

static int compareInts(MapKeyElement n1, MapKeyElement n2) {
    return (*(int *) n1 - *(int *) n2);
}

ChessSystem chessCreate()
{
    ChessSystem chess_system = malloc(sizeof(*chess_system));
    if (NULL == chess_system)
    {
        return NULL;
    }

    chess_system->tournaments = mapCreate(copyDataChar, copyKeyInt, freeChar, freeInt, compareInts);
    if (NULL == chess_system->tournaments)
    {
        free(chess_system->tournaments);
        return NULL;
    }

    return chess_system;
}

void chessDestroy(ChessSystem chess)
{
    mapDestroy(chess->tournaments);
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
