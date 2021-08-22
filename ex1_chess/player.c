#include "player.h"

#include <stdlib.h>
#include <assert.h>

struct player_t {
    int wins;
    int losses;
    int draws;
    int games;
    int play_time;
};

int playerGetWins(Player player)
{
    return player->wins;
}

int playerGetLosses(Player player)
{
    return player->losses;
}

int playerGetDraws(Player player)
{
    return player->draws;
}

int playerGetGames(Player player)
{   
    return player->games;
}

void playerIncreaseWins(Player player)
{
    player->wins++;
}

void playerDecreaseLosses(Player player)
{
    player->losses--;
}

void playerDecreaseDraws(Player player)
{
    player->draws--;
}

MapDataElement playerCopy(MapDataElement n)
{
    if (NULL == n)
    {
        return NULL;
    }

    Player player_source = (Player)n;

    Player player_destination = malloc(sizeof(*player_destination));
    if (!player_destination)
    {
        return NULL;
    }

    *player_destination = *player_source;

    return (MapDataElement)player_destination;
}

void playerFree(MapDataElement n)
{
    if (NULL == n)
    {
        return;
    }

    free(n);
}

bool playerAddOrUpdate(Map players, int player_id, bool is_win, bool is_losse, bool is_draw, int play_time)
{
    Player player = mapGet(players, &player_id);
    if (NULL == player)
    {
        Player_t player;
        player.wins = is_win ? 1 : 0;
        player.losses = is_losse ? 1 : 0;
        player.draws = is_draw ? 1 : 0;
        player.games = 1;
        player.play_time = play_time;

        // ToDo: add if fails
        mapPut(players, &player_id, &player);

        return true;
    }
    else
    {
        player->wins += is_win ? 1 : 0;
        player->losses += is_losse ? 1 : 0;
        player->draws += is_draw ? 1 : 0;
        player->games++;
        player->play_time += play_time;

        return false;
    }
}

void playerSubtract(Player player_1, Player player_2)
{
    player_1->wins -= player_2->wins;
    player_1->losses -= player_2->losses;
    player_1->draws -= player_2->draws;
    player_1->games -= player_2->games;
}

typedef struct player_points_t
{
    int     id;
    double  level;
} PlayerPoints_t, *PlayerPoints;

static double playerGetLevel(Player player)
{
    int number_of_played_games = player->wins + player->losses + player->draws;

    double level = number_of_played_games == 0 ? 0 :
        (double)(6 * player->wins - 10 * player->losses + 2 * player->draws)
        / number_of_played_games;

    return level;
}

static void playerSwap(PlayerPoints player_1, PlayerPoints player_2)
{
    PlayerPoints_t temp = *player_2;
    *player_2 = *player_1;
    *player_1 = temp;
}

static void playerSortPlayersLevel(PlayerPoints players_points, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        // Last i elements are already in place
        for (int j = 0; j < n - i - 1; j++)
        {
            if (players_points[j].level < players_points[j+1].level)
            {
                playerSwap(&players_points[j], &players_points[j+1]);
            }
            else if (players_points[j].level == players_points[j+1].level)
            {
                if (players_points[j].id > players_points[j+1].id)
                {
                    playerSwap(&players_points[j], &players_points[j+1]);
                }
            }
        }
    }
}

bool playerPrintPlayersLevelIdSorted(Map players, FILE* file)
{
    int players_number = mapGetSize(players);

    PlayerPoints players_points = malloc(players_number * sizeof(*players_points));
    if (NULL == players_points)
    {
        return false;
    }

    int* player_id = mapGetFirst(players);

    int i = 0;
    while (NULL != player_id)
    {
        Player player = mapGet(players, player_id);

        double level = playerGetLevel(player);

        players_points[i].id = *player_id;
        players_points[i].level = level;

        free(player_id);
        player_id = mapGetNext(players);
        i++;
        assert(i <= players_number);
    }

    playerSortPlayersLevel(players_points, players_number);

    for (int i = 0; i < players_number; i++)
    {
        // ToDo: should we check if succeeded ?
        fprintf(file, "%d %.2f\n", players_points[i].id, players_points[i].level);
    }

    free(players_points);

    return true;
}

double playerGetAveragePlayTime(Player player)
{
    return player->games == 0 ? 0 : (double)player->play_time / player->games;
}
