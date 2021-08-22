#include "tournament.h"
#include "game.h"
#include "chessSystem.h"
#include "utilities.h"
#include "player.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

struct tournament_t {
    int     max_games_per_player;
    char*   location;
    bool    is_ended;
    Game    games;
    int     number_of_games;
    Map     players;
    int     number_of_players;
    int     winnder_id;
    int     longest_time_game;
    double  average_time_game;
};

MapDataElement tournamentCopy(MapDataElement n)
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

    *tournament_destination = *tournament_souce;
    tournament_destination->games = NULL;

    tournament_destination->location = malloc(strlen(tournament_souce->location) + 1);
    if (NULL == tournament_destination->location)
    {
        return NULL;
    }
    // ToDo: check if should add return code for copy result
    strcpy(tournament_destination->location, tournament_souce->location);

    tournament_destination->players = mapCopy(tournament_souce->players);
    if (NULL == tournament_destination->players)
    {
        free(tournament_destination);
        return NULL;
    }
    
    Game iterator_souce = tournament_souce->games;
    Game* iterator_destination = &(tournament_destination->games);

    while (NULL != iterator_souce)
    {
        *iterator_destination = gameCopy(iterator_souce, NULL);
        // ToDo: check if malloc failed, if yes add logic for deleting all what has already been allocated

        iterator_souce = gameNext(iterator_souce);

        // ToDo: implement somehow different, if it even works...
        iterator_destination = gameGetNextAddress(iterator_souce);
    }

    return (MapDataElement) tournament_destination;
}

void tournamentFree(MapDataElement n)
{
    if (NULL == n)
    {
        return;
    }

    Tournament tournament = (Tournament)n;

    free(tournament->location);

    mapDestroy(tournament->players);

    Game iterator = tournament->games;

    while (NULL != iterator)
    {
        Game iterator_next = gameNext(iterator);

        gameFree(iterator);

        iterator = iterator_next;
    }

    free(tournament);
}

Tournament tournamentCreate(int max_games_per_player, const char* tournament_location)
{
    // ToDo: check if malloc succeeded
    Tournament tournament = malloc(sizeof(*tournament));
    if (NULL == tournament)
    {
        return NULL;
    }

    tournament->max_games_per_player = max_games_per_player;
    tournament->is_ended = false;
    tournament->games = NULL;
    tournament->number_of_games = 0;
    tournament->winnder_id = 0;
    tournament->longest_time_game = 0;
    tournament->average_time_game = 0;
    tournament->number_of_players = 0;

    tournament->location = malloc(strlen(tournament_location) + 1);
    if (NULL == tournament->location)
    {
        return NULL;
    }
    // ToDo: check if should add return code for copy result
    strcpy(tournament->location, tournament_location);

    tournament->players = mapCreate(playerCopy, copyInt, playerFree, freeInt, compareInts);
    if (NULL == tournament->players)
    {
        return NULL;
    }

    return tournament;
}

bool tournamentIsGameExists(Tournament tournament, int first_player, int second_player)
{
    Game game = tournament->games;

    while (NULL != game)
    {
        if (gameIsGameEqual(game, first_player, second_player))
        {
            return true;
        }

        game = gameNext(game);
    }

    return false;
}

bool tournamentCheckLocationValidity(const char* tournament_location)
{
    if ((tournament_location[0] < 'A') || (tournament_location[0] > 'Z'))
    {
        return false;
    }

    for (int i = 1; i < strlen(tournament_location); i++)
    {
        if (!(((tournament_location[i] >= 'a') && (tournament_location[i] <= 'z')) || (' ' == tournament_location[i])))
        {
            return false;
        }        
    }

    return true;    
}

bool tournamentIsPlayersExceededMaxGames(Tournament tournament, int player_id)
{
    int player_games_counter = 0;

    Player player = mapGet(tournament->players, &player_id);
    if (NULL == player)
    {
        player_games_counter = 0;
    }
    else
    {
        player_games_counter = playerGetGames(player);
    }

    if (player_games_counter >= tournament->max_games_per_player)
    {
        return true;
    }

    return false;
}

bool tournamentIsEnded(Tournament tournament)
{
    return tournament->is_ended;   
}

bool tournamentAddGame(Tournament tournament, int first_player, int second_player, Winner winner, int play_time)
{
    Game* iterator = &(tournament->games);

    while (NULL != *iterator)
    {
        // ToDo: consider implementing a list ADT
        iterator = gameGetNextAddress(*iterator);
    }

    *iterator = gameCreate(first_player, second_player, winner, play_time);
    if (NULL == *iterator)
    {
        return false;
    }

    return true;
}

void tournamentUpdateStatisticsOnNewGame(Tournament tournament, int play_time)
{
    if (play_time > tournament->longest_time_game)
    {
        tournament->longest_time_game = play_time;
    }

    tournament->average_time_game =
    (tournament->average_time_game * tournament->number_of_games + play_time)
    / (tournament->number_of_games + 1);

    tournament->number_of_games++;
}

void tournamentIncreaseNumberOfPlayers(Tournament tournament)
{
    tournament->number_of_players++;
}

Map tournamentGetPlayers(Tournament tournament)
{
    return tournament->players;
}

void tournamentSubtractFromGlobalsPlayersStatistics(Tournament tournament, Map players_global)
{
    MAP_FOREACH(int*, player_id, tournament->players)
    {
        Player player_global = mapGet(players_global, player_id);
        assert(player_global != NULL);

        Player player_local = mapGet(tournament->players, player_id);
        assert(player_local != NULL);

        playerSubtract(player_global, player_local);

        freeInt(player_id);
    }
}

void tournamentRemovePlayer(Tournament tournament, int player_id, Map players_global)
{
    if (!tournament->is_ended)
    {
        Game game = tournament->games;

        while (NULL != game)
        {
            gameRemovePlayer(game, player_id, players_global, tournament->players);

            game = gameNext(game);
        }
    }

    // ToDo: decide if should be checked, altough it shoudn't fail
    mapRemove(tournament->players, &player_id);
}

void tournamentPrintStatistics(Tournament tournament, FILE* file)
{
    // ToDo: should we check if fprintf succeeded ?
    fprintf(file, "%d\n", tournament->winnder_id);
    fprintf(file, "%d\n", tournament->longest_time_game);
    fprintf(file, "%.2f\n", tournament->average_time_game);
    fprintf(file, "%s\n", tournament->location);
    fprintf(file, "%d\n", tournament->number_of_games);
    fprintf(file, "%d\n", tournament->number_of_players);
}

int tournamentGetNumberOfGames(Tournament tournament)
{
    return tournament->number_of_games;
}

void tournamentEnd(Tournament tournament)
{
    tournament->is_ended = true;

    // CodeReview: change to MapForEach
    int* player_id = mapGetFirst(tournament->players);

    int max_points = 0;
    int min_losses = 0;
    int max_wins = 0;

    while (NULL != player_id)
    {
        Player player = mapGet(tournament->players, player_id);

        int player_wins = playerGetWins(player);
        int player_losses = playerGetLosses(player);
        int player_draws = playerGetDraws(player);

        int player_points = 2 * player_wins + player_draws;

        if (player_points > max_points)
        {
            max_points = player_points;

            min_losses = player_losses;
            max_wins = player_wins;
            tournament->winnder_id = *player_id;
        }
        else if (player_points == max_points)
        {
            if (player_losses < min_losses)
            {
                min_losses = player_losses;
                max_wins = player_wins;
                tournament->winnder_id = *player_id;
            }
            else if (player_losses == min_losses)
            {
                if (player_wins > max_wins)
                {
                    max_wins = player_wins;
                    tournament->winnder_id = *player_id;
                }
                else if (player_wins == max_wins)
                {
                    if (*player_id < tournament->winnder_id)
                    {
                        tournament->winnder_id = *player_id;
                    }
                }
            }
        }

        free(player_id);
        player_id = mapGetNext(tournament->players);
    }
}
