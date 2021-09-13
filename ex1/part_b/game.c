#include "game.h"
#include "player.h"

#include <stdlib.h>

#define PLAYER_HAS_BEEN_REMOVED -1

struct game_t {
    struct game_t  *next;
    int             first_player;
    int             second_player;
    Winner          winner;
    int             play_time;
};

int gameGetFirstPlayer(Game game)
{
    return game->first_player;
}

int gameGetSecondPlayer(Game game)
{
    return game->second_player;
}

Winner gameGetWinner(Game game)
{
    return game->winner;
}

Game gameGetNext(Game game)
{
    return game->next;
}

Game* gameGetNextAddress(Game game)
{
    return &(game->next);
}

void gameFree(Game game)
{
    free(game);
}

Game gameCopy(Game game, Game next)
{
    Game game_new = malloc(sizeof(*game_new));
    if (NULL == game_new)
    {
        return NULL;
    }

    *game_new = *game;
    game_new->next = next;

    return game;
}

bool gameIsGameEqual(Game game, int first_player, int second_player)
{
    if (((game->first_player == first_player) && (game->second_player == second_player)) ||
        ((game->first_player == second_player) && (game->second_player == first_player)))
    {
        return true;
    }

    return false;
}

Game gameCreate(int first_player, int second_player, Winner winner, int play_time)
{
    Game game = malloc(sizeof(*game));
    if (NULL == game)
    {
        return NULL;
    }

    game->first_player = first_player;
    game->second_player = second_player;
    game->winner = winner;
    game->play_time = play_time;
    game->next = NULL;

    return game;
}

void gameRemovePlayer(Game game, int player_id)
{
    if (game->first_player == player_id)
    {
        game->winner = SECOND_PLAYER;
        game->first_player = PLAYER_HAS_BEEN_REMOVED;
    }
    else if (game->second_player == player_id)
    {
        game->winner = SECOND_PLAYER;
        game->second_player = PLAYER_HAS_BEEN_REMOVED;
    }
}

bool gameIsPlayerLeft(Game game)
{
    return ((game->first_player == PLAYER_HAS_BEEN_REMOVED) || (game->second_player == PLAYER_HAS_BEEN_REMOVED));
}
