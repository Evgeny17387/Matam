#include "Game.h"

using namespace mtm;

#include "Medic.h"

Game::Game(int height, int width): height(height), width(width), board(width, std::vector<Character*>(height))
{
    // ToDo: is that correct ? since the board init might throw exeption as well
    if ((height <= 0) || (width <= 0))
    {
        throw IllegalArgument();
    }
}

Game::~Game()
{
    // ToDo: implement
}

Game::Game(const Game& game)
{
    this->height = game.height;
    this->width = game.width;

    throw NotImplementedYet();
}

Game& Game::operator=(const Game& game)
{
    this->height = game.height;
    this->width = game.width;

    throw NotImplementedYet();

    return *this;
}

Character* Game::makeCharacter(CharacterType type, Team team, units_t health, units_t ammo, units_t range, units_t power)
{
    if ((health <= 0) || (ammo < 0) || (range < 0) || (power < 0))
    {
        throw IllegalArgument();
    }

    switch (type)
    {
    case CharacterType::MEDIC:
        return new Medic(team, health, ammo, range, power);
        break;
    
    default:
        // ToDo: what should be the default ?
        return new Character(team, health, ammo, range, power, 0, 0);
        break;
    }
}

void Game::addCharacter(const GridPoint& coordinates, Character *character)
{
    verifyLegalCoordinates(coordinates);
    verifyCellEmpty(coordinates);

    this->board[coordinates.col][coordinates.row] = character;
}

void Game::move(const GridPoint& src_coordinates, const GridPoint& dst_coordinates)
{
    verifyLegalCoordinates(src_coordinates);
    verifyLegalCoordinates(dst_coordinates);
    verifyCellNotEmpty(src_coordinates);

    if (GridPoint::distance(src_coordinates, dst_coordinates) > this->board[src_coordinates.col][src_coordinates.row]->getMoveRange())
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

void Game::attack(const GridPoint& src_coordinates, const GridPoint& dst_coordinates)
{
    verifyLegalCoordinates(src_coordinates);
    verifyLegalCoordinates(dst_coordinates);
    verifyCellNotEmpty(src_coordinates);


}

bool Game::isOver(Team* winningTeam) const
{
    int power_lifters_counter = 0;
    int cross_fitters_counter = 0;

    // ToDo: implement generic iteration over board with some apply, to use also in operator<<
    // ToDo: also replace with foreach
    for (int row = 0; row < this->height; row++)
    {
        for (int col = 0; col < this->width; col++)
        {
            if (this->board[col][row] != NULL)
            {
                // ToDo: i don't like this, maybe implement as array of counters
                if (this->board[col][row]->getTeam() == Team::POWERLIFTERS)
                {
                    power_lifters_counter++;
                }
                else if (this->board[col][row]->getTeam() == Team::CROSSFITTERS)
                {
                    cross_fitters_counter++;
                }
            }
        }
    }

    if ((power_lifters_counter != 0) && (cross_fitters_counter == 0))
    {
        if (winningTeam != NULL)
        {
            *winningTeam = Team::POWERLIFTERS;
        }
        return true;
    }
    else if ((cross_fitters_counter != 0) && (power_lifters_counter == 0))
    {
        if (winningTeam != NULL)
        {
            *winningTeam = Team::CROSSFITTERS;
        }
        return true;
    }

    return false;
}

// ToDo: should it be used this way ?
namespace mtm
{
    // ToDo: why it has to be friend ?
    ostream& operator<<(ostream& os, const Game& game)
    {
        // ToDo: replace with string
        char *begin = new char[game.width * game.height];

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
}

void Game::verifyLegalCoordinates(const GridPoint& coordinates) const
{
    if ((coordinates.row < 0) || (coordinates.col < 0) || (coordinates.row >= this->height) || (coordinates.col >= this->width))
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
