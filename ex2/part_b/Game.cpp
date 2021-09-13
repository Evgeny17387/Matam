#include "Game.h"
#include "Exceptions.h"
#include "Soldier.h"
#include "Medic.h"
#include "Sniper.h"

using std::vector;
using std::ostream;
using std::shared_ptr;

namespace mtm
{
    Game::Game(int height, int width): height(height), width(width)
    {
        if ((height <= 0) || (width <= 0))
        {
            throw IllegalArgument();
        }

        this->power_lifters_counter = 0;
        this->cross_fitters_counter = 0;

        this->board = vector<vector<shared_ptr<Character>>>(this->width,
            vector<shared_ptr<Character>>(this->height, NULL));
    }

    void Game::copyGame(const Game& game)
    {
        this->board = vector<vector<shared_ptr<Character>>>(this->width,
            vector<shared_ptr<Character>>(this->height, NULL));

        for (int row = 0; row < this->height; row++)
        {
            for (int col = 0; col < this->width; col++)
            {
                if (game.board[col][row] != NULL)
                {
                    this->board[col][row] = shared_ptr<Character>(game.board[col][row]->clone());
                }
            }
        }
    }

    Game::Game(const Game& game): height(game.height), width(game.width),
        power_lifters_counter(game.power_lifters_counter), cross_fitters_counter(game.cross_fitters_counter)
    {
        copyGame(game);
    }

    Game& Game::operator=(const Game& game)
    {
        if (this == &game)
        {
            return *this;
        }

        this->board.clear();

        this->height = game.height;
        this->width = game.width;
        this->power_lifters_counter = game.power_lifters_counter;
        this->cross_fitters_counter = game.cross_fitters_counter;

        copyGame(game);

        return *this;
    }

    shared_ptr<Character> Game::makeCharacter(CharacterType type, Team team, units_t health, units_t ammo,
        units_t range, units_t power)
    {
        if ((health <= 0) || (ammo < 0) || (range < 0) || (power < 0))
        {
            throw IllegalArgument();
        }

        switch (type)
        {
        case CharacterType::SOLDIER:
            return std::shared_ptr<Character>(new Soldier(team, health, ammo, range, power));
            break;

        case CharacterType::MEDIC:
            return std::shared_ptr<Character>(new Medic(team, health, ammo, range, power));
            break;

        case CharacterType::SNIPER:
            return std::shared_ptr<Character>(new Sniper(team, health, ammo, range, power));
            break;

        default:
            throw IllegalArgument();
            break;
        }
    }

    void Game::addCharacter(const GridPoint& coordinates, shared_ptr<Character> character)
    {
        verifyLegalCoordinates(coordinates);
        verifyCellEmpty(coordinates);

        this->board[coordinates.col][coordinates.row] = character;

        if (character.get()->getTeam() == Team::POWERLIFTERS)
        {
            this->power_lifters_counter++;
        }
        else if (character.get()->getTeam() == Team::CROSSFITTERS)
        {
            this->cross_fitters_counter++;
        }
    }

    void Game::move(const GridPoint& src_coordinates, const GridPoint& dst_coordinates)
    {
        verifyLegalCoordinates(src_coordinates);
        verifyLegalCoordinates(dst_coordinates);
        verifyCellNotEmpty(src_coordinates);

        if (GridPoint::distance(src_coordinates, dst_coordinates) >
            this->board[src_coordinates.col][src_coordinates.row]-> getMoveRange())
        {
            throw MoveTooFar();
        }

        verifyCellEmpty(dst_coordinates);

        this->board[dst_coordinates.col][dst_coordinates.row] = this->board[src_coordinates.col][src_coordinates.row];
        this->board[src_coordinates.col][src_coordinates.row] = NULL;
    }

    void Game::reload(const GridPoint& coordinates)
    {
        verifyLegalCoordinates(coordinates);
        verifyCellNotEmpty(coordinates);

        this->board[coordinates.col][coordinates.row]->reload();
    }

    void Game::updatePlayersStatus()
    {
        for (int row = 0; row < this->height; row++)
        {
            for (int col = 0; col < this->width; col++)
            {
                shared_ptr<Character> defender = board[col][row];

                if (defender == NULL)
                {
                    continue;
                }

                if (defender->getHealth() <= 0)
                {
                    if (defender.get()->getTeam() == Team::POWERLIFTERS)
                    {
                        this->power_lifters_counter--;
                    }
                    else if (defender.get()->getTeam() == Team::CROSSFITTERS)
                    {
                        this->cross_fitters_counter--;
                    }

                    defender.reset();
                    this->board[col][row] = NULL;
                }
            }

        }
    }

    void Game::attack(const GridPoint& src_coordinates, const GridPoint& dst_coordinates)
    {
        verifyLegalCoordinates(src_coordinates);
        verifyLegalCoordinates(dst_coordinates);
        verifyCellNotEmpty(src_coordinates);

        shared_ptr<Character> attacker = this->board[src_coordinates.col][src_coordinates.row];
        shared_ptr<Character> defender = this->board[dst_coordinates.col][dst_coordinates.row];

        if (!attacker->isAttackInRange(src_coordinates, dst_coordinates))
        {
            throw OutOfRange();
        }

        if (!attacker->isEnoughAmmo(defender.get()))
        {
            throw OutOfAmmo();
        }

        if (!attacker->canAttack(defender.get(), src_coordinates, dst_coordinates))
        {
            throw IllegalTarget();
        }

        attacker->attack(this->board, dst_coordinates);

        updatePlayersStatus();
    }

    bool Game::isOver(Team* winningTeam) const
    {
        if ((this->power_lifters_counter != 0) && (this->cross_fitters_counter == 0))
        {
            if (winningTeam != NULL)
            {
                *winningTeam = Team::POWERLIFTERS;
            }
            return true;
        }
        else if ((this->cross_fitters_counter != 0) && (this->power_lifters_counter == 0))
        {
            if (winningTeam != NULL)
            {
                *winningTeam = Team::CROSSFITTERS;
            }
            return true;
        }

        return false;
    }

    ostream& operator<<(ostream& os, const Game& game)
    {
        char* begin = new char[game.width * game.height];

        for (int row = 0; row < game.height; row++)
        {
            for (int col = 0; col < game.width; col++)
            {
                if (game.board[col][row] != NULL)
                {
                    begin[row * game.width + col] = game.board[col][row]->getSymbol();
                }
                else
                {
                    begin[row * game.width + col] = ' ';
                }
            }
        }

        printGameBoard(os, begin, begin + game.width * game.height, game.width);

        delete[] begin;

        return os;
    }

    void Game::verifyLegalCoordinates(const GridPoint& coordinates) const
    {
        if ((coordinates.row < 0) || (coordinates.col < 0) ||
            (coordinates.row >= this->height) || (coordinates.col >= this->width))
        {
            throw IllegalCell();
        }
    }

    void Game::verifyCellNotEmpty(const GridPoint& coordinates) const
    {
        if (this->board[coordinates.col][coordinates.row] == NULL)
        {
            throw CellEmpty();
        }
    }

    void Game::verifyCellEmpty(const GridPoint& coordinates) const
    {
        if (this->board[coordinates.col][coordinates.row] != NULL)
        {
            throw CellOccupied();
        }
    }

}
