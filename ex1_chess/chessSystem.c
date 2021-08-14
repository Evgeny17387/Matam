#include "chessSystem.h"
#include "mtm_map/map.h"

#include <stdlib.h>

// ToDo: check if should be 0 or 1
#define MIN_GAMES_PER_PLAYER 1

// ToDo: add location member
typedef struct tournament_t {
    int max_games_per_player;
} Tournament_t, *Tournament;

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

static MapDataElement copyDataTournament(MapDataElement n)
{
    // ToDo: update once Tournment struct is modified
    if (!n)
    {
        return NULL;
    }

    Tournament copy = malloc(sizeof(*copy));
    if (!copy)
    {
        return NULL;
    }

    *copy = *(Tournament) n;

    return (MapDataElement) copy;
}

static void freeInt(MapKeyElement n)
{
    free(n);
}

static void freeTournament(MapDataElement n)
{
    // ToDo: update once Tournment struct is modified
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

    chess_system->tournaments = mapCreate(copyDataTournament, copyKeyInt, freeTournament, freeInt, compareInts);
    if (NULL == chess_system->tournaments)
    {
        mapDestroy(chess_system->tournaments);
        return NULL;
    }

    return chess_system;
}

void chessDestroy(ChessSystem chess)
{
    // ToDO: update once chess struct is modified
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

    Tournament_t tournament;
    tournament.max_games_per_player = max_games_per_player;

    MapResult map_result = mapPut(chess->tournaments, &tournament_id, &tournament);
    if (MAP_OUT_OF_MEMORY == map_result)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    return CHESS_SUCCESS;
}
