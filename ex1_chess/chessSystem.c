#include "chessSystem.h"
#include "mtm_map/map.h"
#include "utilities.h"
#include "tournament.h"
#include "player.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MIN_VALID_GAMES_PER_PLAYER  1
#define MIN_VALID_TOURNMENT_ID      1
#define MIN_VALID_PLAYER_ID         1
#define MIN_VALID_PLAY_TIME         0

#define INVALID_AVERAGE_TIME        -1

struct chess_system_t {
    Map tournaments;
    Map players;
};

static inline bool chessCheckIdValidity(int tournament_id, int first_player, int second_player)
{
    if ((tournament_id < MIN_VALID_TOURNMENT_ID) ||
        (first_player < MIN_VALID_PLAYER_ID) ||
        (second_player < MIN_VALID_PLAYER_ID) ||
        (first_player == second_player))
    {
        return false;
    }

    return true;
}

ChessSystem chessCreate()
{
    ChessSystem chess_system = malloc(sizeof(*chess_system));
    if (NULL == chess_system)
    {
        return NULL;
    }

    chess_system->tournaments = mapCreate(tournamentCopy, copyInt, tournamentFree, freeInt, compareInts);
    if (NULL == chess_system->tournaments)
    {
        free(chess_system);
        return NULL;
    }

    chess_system->players = mapCreate(playerCopy, copyInt, playerFree, freeInt, compareInts);
    if (NULL == chess_system->players)
    {
        mapDestroy(chess_system->tournaments);
        free(chess_system);
        return NULL;
    }

    return chess_system;
}

void chessDestroy(ChessSystem chess)
{
    if (NULL == chess)
    {
        return;
    }

    mapDestroy(chess->tournaments);
    mapDestroy(chess->players);
    free(chess);
}

ChessResult chessAddTournament(ChessSystem chess, int tournament_id, int max_games_per_player, const char* tournament_location)
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

    if (!tournamentCheckLocationValidity(tournament_location))
    {
        return CHESS_INVALID_LOCATION;
    }

    if (max_games_per_player < MIN_VALID_GAMES_PER_PLAYER)
    {
        return CHESS_INVALID_MAX_GAMES;
    }

    Tournament tournament = tournamentCreate(max_games_per_player, tournament_location);
    if (NULL == tournament)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    MapResult map_result = mapPut(chess->tournaments, &tournament_id, tournament);
    if (MAP_OUT_OF_MEMORY == map_result)
    {
        tournamentFree(tournament);
        return CHESS_OUT_OF_MEMORY;
    }

    tournamentFree(tournament);

    return CHESS_SUCCESS;
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player, int second_player, Winner winner, int play_time)
{
    if (NULL == chess)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (!chessCheckIdValidity(tournament_id, first_player, second_player))
    {
        return CHESS_INVALID_ID;
    }

    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    if (NULL == tournament)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }

    if (tournamentIsEnded(tournament))
    {
        return CHESS_TOURNAMENT_ENDED;
    }

    if (tournamentIsGameExists(tournament, first_player, second_player))
    {
        return CHESS_GAME_ALREADY_EXISTS;
    }

    if (play_time < MIN_VALID_PLAY_TIME)
    {
        return CHESS_INVALID_PLAY_TIME;
    }

    if (tournamentIsPlayersExceededMaxGames(tournament, first_player) ||
        tournamentIsPlayersExceededMaxGames(tournament, second_player))
    {
        return CHESS_EXCEEDED_GAMES;
    }

    if (!tournamentAddGame(tournament, first_player, second_player, winner, play_time))
    {
        return CHESS_OUT_OF_MEMORY;
    }

    // ToDo: add if fails for both
    playerAddOrUpdate(chess->players, first_player, FIRST_PLAYER == winner, SECOND_PLAYER == winner, DRAW == winner, play_time);
    playerAddOrUpdate(chess->players, second_player, SECOND_PLAYER == winner, FIRST_PLAYER == winner, DRAW == winner, play_time);

    return CHESS_SUCCESS;
}

ChessResult chessRemoveTournament(ChessSystem chess, int tournament_id)
{
    if (NULL == chess)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (tournament_id < MIN_VALID_TOURNMENT_ID)
    {
        return CHESS_INVALID_ID;
    }

    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    if (NULL == tournament)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }

    Map players_tournament = tournamentGetPlayers(tournament);

    MAP_FOREACH(int*, player_id, players_tournament)
    {
        Player player_global = mapGet(chess->players, player_id);
        assert(player_global != NULL);

        Player player_local = mapGet(players_tournament, player_id);
        assert(player_local != NULL);

        playerSubtract(player_global, player_local);

        freeInt(player_id);
    }

    // ToDo: decide if check needed
    mapRemove(chess->tournaments, &tournament_id);

    return CHESS_SUCCESS;
}

ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    if (NULL == chess)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (player_id < MIN_VALID_PLAYER_ID)
    {
        return CHESS_INVALID_ID;
    }

    Player player = (Player)mapGet(chess->players, &player_id);
    if (NULL == player)
    {
        return CHESS_PLAYER_NOT_EXIST;
    }

    MAP_FOREACH(int*, tournament_id, chess->tournaments)
    {
        Tournament tournament = mapGet(chess->tournaments, tournament_id);

        tournamentRemovePlayer(tournament, player_id, chess->players);
        
        freeInt(tournament_id);
    }

    // ToDo: maybe to add assert, tough it should always succeed
    mapRemove(chess->players, &player_id);

    return CHESS_SUCCESS;
}

ChessResult chessEndTournament(ChessSystem chess, int tournament_id)
{
    if (NULL == chess)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (tournament_id < MIN_VALID_TOURNMENT_ID)
    {
        return CHESS_INVALID_ID;
    }

    // ToDo: add casting for mapGet, maybe other APIs as well
    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    if (NULL == tournament)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }

    if (tournamentIsEnded(tournament))
    {
        return CHESS_TOURNAMENT_ENDED;
    }

    if (0 == tournamentGetNumberOfGames(tournament))
    {
        return CHESS_NO_GAMES;
    }

    tournamentEnd(tournament);

    return CHESS_SUCCESS;
}

double chessCalculateAveragePlayTime(ChessSystem chess, int player_id, ChessResult* chess_result)
{
    double average_play_time = INVALID_AVERAGE_TIME;

    if (NULL == chess_result)
    {
        return average_play_time;
    }

    if (NULL == chess)
    {
        *chess_result = CHESS_NULL_ARGUMENT;
        return average_play_time;
    }

    if (player_id < MIN_VALID_PLAYER_ID)
    {
        *chess_result = CHESS_INVALID_ID;
        return average_play_time;
    }

    Player player = (Player)mapGet(chess->players, &player_id);
    if (NULL == player)
    {
        *chess_result = CHESS_PLAYER_NOT_EXIST;
        return average_play_time;
    }

    *chess_result = CHESS_SUCCESS;
    return playerGetAveragePlayTime(player);
}

ChessResult chessSavePlayersLevels(ChessSystem chess, FILE* file)
{
    if (NULL == chess)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (NULL == file)
    {
        return CHESS_SAVE_FAILURE;
    }

    if (!playerPrintPlayersLevelIdSorted(chess->players, file))
    {
        return CHESS_SAVE_FAILURE;
    }

    return CHESS_SUCCESS;
}

ChessResult chessSaveTournamentStatistics(ChessSystem chess, char* path_file)
{
    if ((NULL == chess) || (NULL == path_file))
    {
        return CHESS_NULL_ARGUMENT;
    }

    FILE* file = fopen(path_file, "w");
    if (NULL == file)
    {
        return CHESS_SAVE_FAILURE;
    }

    bool is_at_least_one_tournment = false;

    MAP_FOREACH(int*, tournament_id, chess->tournaments)
    {
        Tournament tournament = mapGet(chess->tournaments, tournament_id);

        if (tournamentIsEnded(tournament))
        {
            is_at_least_one_tournment = true;

            tournamentPrintStatistics(tournament, file);
        }

        freeInt(tournament_id);
    }

    fclose(file);

    if (!is_at_least_one_tournment)
    {
        return CHESS_NO_TOURNAMENTS_ENDED;
    }

    return CHESS_SUCCESS;
}
