#include "chessSystem.h"
#include "mtm_map/map.h"

#include <stdlib.h>

// ToDo: check if should be 0 or 1
#define MIN_VALID_GAMES_PER_PLAYER 1
#define MIN_VALID_TOURNMENT_ID 1
#define MIN_VALID_PLAYER_ID 1
#define MIN_VALID_PLAY_TIME 0

typedef struct game_t {
    struct game_t * next;
    int first_player;
    int second_player;
    Winner winner;
    int play_time;
} Game_t, *Game;

// ToDo: add location member
typedef struct tournament_t {
    int max_games_per_player;
    bool is_ended;
    Game games;
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

    if (tournament_id < MIN_VALID_TOURNMENT_ID)
    {
        return CHESS_INVALID_ID;
    }

    if (NULL != mapGet(chess->tournaments, &tournament_id))
    {
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }

    // ToDo: check for location

    if (max_games_per_player < MIN_VALID_GAMES_PER_PLAYER)
    {
        return CHESS_INVALID_MAX_GAMES;
    }

    Tournament_t tournament;
    tournament.max_games_per_player = max_games_per_player;
    tournament.is_ended = false;
    tournament.games = NULL;

    MapResult map_result = mapPut(chess->tournaments, &tournament_id, &tournament);
    if (MAP_OUT_OF_MEMORY == map_result)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    return CHESS_SUCCESS;
}

bool isGameExists(Tournament tournament, Game game)
{
    Game iterator = tournament->games;

    while (NULL != iterator)
    {
        if (((iterator->first_player == game->first_player) && (iterator->second_player == game->second_player)) ||
            ((iterator->first_player == game->second_player) && (iterator->second_player == game->first_player)))
        {
            return true;            
        }

        iterator = iterator->next;
    }

    return false;
}

bool isPlayersExceededMaxGames(Tournament tournament, Game game)
{
    Game iterator = tournament->games;

    int first_player_games_counter = 0;
    int second_player_games_counter = 0;

    while (NULL != iterator)
    {
        if ((iterator->first_player == game->first_player) || (iterator->second_player == game->first_player))
        {
            first_player_games_counter++;
        }

        if ((iterator->first_player == game->second_player) || (iterator->second_player == game->second_player))
        {
            second_player_games_counter++;
        }

        iterator = iterator->next;
    }

    if ((first_player_games_counter >= tournament->max_games_per_player) || (second_player_games_counter >= tournament->max_games_per_player))
    {
        return true;
    }

    return false;
}

void addGame(Tournament tournament, Game game)
{
    Game* iterator = &(tournament->games);

    while (NULL != *iterator)
    {
        iterator = &((*iterator)->next);
    }

    *iterator = malloc(sizeof(**iterator));
    // ToDo: add check for malloc
    **iterator = *game;
}

ChessResult chessAddGame(
    ChessSystem chess,
    int tournament_id,
    int first_player,
    int second_player,
    Winner winner,
    int play_time)
{
    if (NULL == chess)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if ((tournament_id < MIN_VALID_TOURNMENT_ID) ||
        (first_player < MIN_VALID_PLAYER_ID) ||
        (second_player < MIN_VALID_PLAYER_ID) ||
        (first_player == second_player) ||
        ((FIRST_PLAYER != winner) && (SECOND_PLAYER != winner) && (DRAW != winner)))
    {
        return CHESS_INVALID_ID;
    }

    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    if (NULL == tournament)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }

    if (tournament->is_ended)
    {
        return CHESS_TOURNAMENT_ENDED;
    }

    Game_t game;
    game.next = NULL;
    game.first_player = first_player;
    game.second_player = second_player;
    game.winner = winner;
    game.play_time = play_time;

    if (isGameExists(tournament, &game))
    {
        return CHESS_GAME_ALREADY_EXISTS;
    }

    if (play_time < MIN_VALID_PLAY_TIME)
    {
        return CHESS_INVALID_PLAY_TIME;
    }

    if (isPlayersExceededMaxGames(tournament, &game))
    {
        return CHESS_EXCEEDED_GAMES;
    }

    addGame(tournament, &game);

    return CHESS_SUCCESS;
}
