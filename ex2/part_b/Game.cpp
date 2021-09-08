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
        return new Character(team, health, ammo, range, power);
        break;
    }
}

void Game::addCharacter(const GridPoint& coordinates, Character *character)
{
    if ((coordinates.row < 0) || (coordinates.col < 0) || (coordinates.row >= this->height) || (coordinates.col >= this->width))
    {
        throw IllegalCell();
    }

    if (this->board[coordinates.col][coordinates.row] != NULL)
    {
        throw CellOccupied();
    }

    this->board[coordinates.col][coordinates.row] = character;
}

void Game::move(const GridPoint& src_coordinates, const GridPoint& dst_coordinates)
{
    // ToDo: write as private function, also use the check in addCharacter
    if ((src_coordinates.row < 0) || (src_coordinates.col < 0) || (src_coordinates.row >= this->height) || (src_coordinates.col >= this->width))
    {
        throw IllegalCell();
    }

    if ((dst_coordinates.row < 0) || (dst_coordinates.col < 0) || (dst_coordinates.row >= this->height) || (dst_coordinates.col >= this->width))
    {
        throw IllegalCell();
    }

    if (this->board[src_coordinates.col][src_coordinates.row] == NULL)
    {
        throw CellEmpty();
    }

    // ToDo: add check for character move capacity

    if (this->board[dst_coordinates.col][dst_coordinates.row] != NULL)
    {
        throw CellOccupied();
    }

    this->board[dst_coordinates.col][dst_coordinates.row] = this->board[src_coordinates.col][src_coordinates.row];
    this->board[src_coordinates.col][src_coordinates.row] = NULL;
}

// ToDo: should it be used this way ?
namespace mtm
{
    // ToDo: why it has to be friend ?
    ostream& operator<<(ostream& os, const Game& game)
    {
        char *begin = new char[game.width * game.height];

        for (int row = 0; row < game.height; row++)
        {
            for (int col = 0; col < game.width; col++)
            {
                if (game.board[col][row] != NULL)
                {
                    begin[row * game.width + col] = game.board[col][row]->GetSymbol();
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
