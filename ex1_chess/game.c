#include "game.h"
#include "player.h"

#include <stdlib.h>

struct game_t {
    struct game_t  *next;
    int             first_player;
    int             second_player;
    Winner          winner;
    int             play_time;
};

Game gameNext(Game game)
{
    return game->next;
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

Game* gameGetNextAddress(Game game)
{
    return &(game->next);
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

static void gameUpdatePlayerStatistics(Map players, int player_id, bool is_last_time_player_lost, bool is_last_time_was_draw)
{
    Player player = mapGet(players, &player_id);
    if (NULL != player)
    {
        if (is_last_time_player_lost)
        {
            playerDecreaseLosses(player);
            playerIncreaseWins(player);
        }
        else if (is_last_time_was_draw)
        {
            playerDecreaseDraws(player);
            playerIncreaseWins(player);
        }
    }
}

void gameRemovePlayer(Game game, int player_id, Map players_global, Map players_local)
{
    if (game->first_player == player_id)
    {
        // CodeReview: Add note for what the function returns
        // CodeReview: change logic to update winner all the time
        gameUpdatePlayerStatistics(players_global, game->second_player, FIRST_PLAYER == game->winner, DRAW == game->winner);
        gameUpdatePlayerStatistics(players_local, game->second_player, FIRST_PLAYER == game->winner, DRAW == game->winner);

        game->winner = SECOND_PLAYER;
        game->first_player = -1;
    }
    else if (game->second_player == player_id)
    {
        gameUpdatePlayerStatistics(players_global, game->first_player, SECOND_PLAYER == game->winner, DRAW == game->winner);
        gameUpdatePlayerStatistics(players_local, game->first_player, SECOND_PLAYER == game->winner, DRAW == game->winner);

        // CodeReview: add define of player whic removed
        game->winner = SECOND_PLAYER;
        game->second_player = -1;
    }
}
