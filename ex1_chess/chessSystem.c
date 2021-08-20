#include "chessSystem.h"
#include "mtm_map/map.h"

#include <stdlib.h>

// ToDo: check if should be 0 or 1
#define MIN_VALID_GAMES_PER_PLAYER  1
#define MIN_VALID_TOURNMENT_ID      1
#define MIN_VALID_PLAYER_ID         1
#define MIN_VALID_PLAY_TIME         0

#define SIZE_EMPY                   0

#define PRINT_TO_FILE_SUCCESS       0

#define NO_WINNER_DECLARED_YET      -1

typedef struct game_t {
    struct game_t  *next;
    int             first_player;
    int             second_player;
    Winner          winner;
    int             play_time;
} Game_t, *Game;

// ToDo: add location member
typedef struct tournament_t {
    int     max_games_per_player;
    bool    is_ended;
    Game    games;
    int     winnder_id;
} Tournament_t, *Tournament;

typedef struct player_t {
    int wins;
    int losses;
    int draws;
} Player_t, *Player;

struct chess_system_t {
    Map tournaments;
    Map players;
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
    Tournament tournament_souce = (Tournament)n;

    if (!tournament_souce)
    {
        return NULL;
    }

    Tournament tournament_destination = malloc(sizeof(*tournament_destination));
    if (!tournament_destination)
    {
        return NULL;
    }

    tournament_destination->is_ended = tournament_souce->is_ended;
    tournament_destination->max_games_per_player = tournament_souce->max_games_per_player;
    tournament_destination->games = NULL;
    tournament_destination->winnder_id = tournament_souce->winnder_id;

    Game iterator_souce = tournament_souce->games;
    Game* iterator_destination = &(tournament_destination->games);

    while (NULL != iterator_souce)
    {
        *iterator_destination = malloc(sizeof(**iterator_destination));
        // ToDo: check if malloc failed, if yes add logic for deleting all what has already been allocated

        (*iterator_destination)->first_player = iterator_souce->first_player;
        (*iterator_destination)->second_player = iterator_souce->second_player;
        (*iterator_destination)->winner = iterator_souce->winner;
        (*iterator_destination)->play_time = iterator_souce->play_time;
        (*iterator_destination)->next = NULL;

        iterator_souce = iterator_souce->next;
        iterator_destination = &((*iterator_destination)->next);
    }

    return (MapDataElement) tournament_destination;
}

static void freeInt(MapKeyElement n)
{
    free(n);
}

static void freeTournament(MapDataElement n)
{
    Tournament tournament = (Tournament)n;

    Game iterator = tournament->games;

    while (NULL != iterator)
    {
        Game iterator_next = iterator->next;

        free(iterator);

        iterator = iterator_next;
    }

    free(n);
}

static int compareInts(MapKeyElement n1, MapKeyElement n2) {
    return (*(int *) n1 - *(int *) n2);
}

static MapDataElement copyDataPlayer(MapDataElement n)
{
    Player player_source = (Player)n;

    if (!player_source)
    {
        return NULL;
    }

    Player player_destination = malloc(sizeof(*player_destination));
    if (!player_destination)
    {
        return NULL;
    }

    player_destination->wins = player_source->wins;
    player_destination->losses = player_source->losses;
    player_destination->draws = player_source->draws;

    return (MapDataElement)player_destination;
}

static void freePlayer(MapDataElement n)
{
    free(n);
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
        return NULL;
    }

    chess_system->players = mapCreate(copyDataPlayer, copyKeyInt, freePlayer, freeInt, compareInts);
    if (NULL == chess_system->players)
    {
        mapDestroy(chess_system->tournaments);
        return NULL;
    }

    return chess_system;
}

void chessDestroy(ChessSystem chess)
{
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

    // ToDo: check for location

    if (max_games_per_player < MIN_VALID_GAMES_PER_PLAYER)
    {
        return CHESS_INVALID_MAX_GAMES;
    }

    Tournament_t tournament;
    tournament.max_games_per_player = max_games_per_player;
    tournament.is_ended = false;
    tournament.games = NULL;
    tournament.winnder_id = NO_WINNER_DECLARED_YET;

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

bool addGame(Tournament tournament, Game game)
{
    Game* iterator = &(tournament->games);

    while (NULL != *iterator)
    {
        iterator = &((*iterator)->next);
    }

    *iterator = malloc(sizeof(**iterator));
    if (NULL == *iterator)
    {
        return false;
    }

    **iterator = *game;

    return true;
}

static void addPlayer(Map players, int player_id, bool is_win, bool is_losse, bool is_draw)
{
    Player player = mapGet(players, &player_id);
    if (NULL == player)
    {
        Player_t player;
        player.wins = is_win ? 1 : 0;
        player.losses = is_losse ? 1 : 0;
        player.draws = is_draw ? 1 : 0;

        // ToDo: add if fails
        mapPut(players, &player_id, &player);
    }
    else
    {
        player->wins += is_win ? 1 : 0;
        player->losses += is_losse ? 1 : 0;
        player->draws += is_draw ? 1 : 0;
    }
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player, int second_player, Winner winner, int play_time)
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

    if (!addGame(tournament, &game))
    {
        return CHESS_OUT_OF_MEMORY;
    }

    // ToDo: add if fails for both
    addPlayer(chess->players, first_player, FIRST_PLAYER == winner, SECOND_PLAYER == winner, DRAW == winner);
    addPlayer(chess->players, second_player, SECOND_PLAYER == winner, FIRST_PLAYER == winner, DRAW == winner);

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

    if (MAP_ITEM_DOES_NOT_EXIST == mapRemove(chess->tournaments, &tournament_id))
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }

    // ToDo: update statistics

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

    Tournament tournament = mapGetFirst(chess->tournaments);

    while (NULL != tournament)
    {
        if (false == tournament->is_ended)
        {
            Game game = tournament->games;

            while (NULL != game)
            {
                if (game->first_player == player_id)
                {
                    game->winner = SECOND_PLAYER;
                }
                else if (game->second_player == player_id)
                {
                    game->winner = FIRST_PLAYER;
                }

                game = game->next;
            }
        }

        freeInt(tournament);
        tournament = mapGetNext(chess->tournaments);
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

    if (tournament->is_ended)
    {
        return CHESS_TOURNAMENT_ENDED;
    }

    // ToDo: maybe add games counter
    if (NULL == tournament->games)
    {
        return CHESS_NO_GAMES;
    }

    tournament->is_ended = true;

    // ToDo: add tournment winner calculation

    return CHESS_SUCCESS;
}

double chessCalculateAveragePlayTime(ChessSystem chess, int player_id, ChessResult* chess_result)
{
    double average_play_time = 0;

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

    // ToDo: think of how to implement when this statistics is always being kept upadted

    int played_games = 0;

    int* tournament_id = mapGetFirst(chess->tournaments);

    while (NULL != tournament_id)
    {
        Tournament tournament = mapGet(chess->tournaments, tournament_id);

        if (false == tournament->is_ended)
        {
            Game game = tournament->games;

            while (NULL != game)
            {
                if ((game->first_player == player_id) || (game->second_player == player_id))
                {
                    played_games++;
                    average_play_time += game->play_time;
                }

                game = game->next;
            }
        }

        freeInt(tournament_id);
        tournament_id = mapGetNext(chess->tournaments);
    }

    average_play_time = played_games != 0 ? average_play_time / played_games : average_play_time;

    *chess_result = CHESS_SUCCESS;
    return average_play_time;
}

ChessResult chessSavePlayersLevels(ChessSystem chess, FILE* file)
{
    if (NULL == chess)
    {
        return CHESS_NULL_ARGUMENT;
    }

    int* player_id = mapGetFirst(chess->players);

    // ToDo: write players in a specific order

    while (NULL != player_id)
    {
        Player player = mapGet(chess->players, player_id);

        // ToDo: make sure total number of player games can't be 0
        double level = (6 * player->wins - 10 * player->losses + 2 * player->draws) / (player->wins + player->losses + player->draws);
    
        // ToDo: should we check if succeeded ?
        fprintf(file, "%d %f\n", *player_id, level);

        freeInt(player_id);
        player_id = mapGetNext(chess->players);
    }

    return CHESS_SUCCESS;
}

ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file)
{
    if (NULL == chess)
    {
        return CHESS_NULL_ARGUMENT;
    }

    FILE* file = fopen(path_file, "w");
    if (NULL == file)
    {
        return CHESS_SAVE_FAILURE;
    }

    int* tournament_id = mapGetFirst(chess->tournaments);

    bool is_at_least_one_tournment = false;

    while (NULL != tournament_id)
    {
        Tournament tournament = mapGet(chess->tournaments, tournament_id);

        if (tournament->is_ended)
        {
            is_at_least_one_tournment = true;

            // ToDo: add all other parameters

            // ToDo: should we check if succeeded ?
            fprintf(file, "%d %d\n", *tournament_id, tournament->winnder_id);
        }

        freeInt(tournament_id);
        tournament_id = mapGetNext(chess->tournaments);
    }

    fclose(file);

    if (!is_at_least_one_tournment)
    {
        return CHESS_NO_TOURNAMENTS_ENDED;
    }

    return CHESS_SUCCESS;
}
