#include "Game.h"

using namespace mtm;

#include "Medic.h"

Game::Game(int height, int width): height(height), width(width), board(width, std::vector<Character*>(height))
{
}

Game::~Game()
{
    // ToDo: implement
}

Game::Game(const Game& game)
{
    this->height = game.height;
    this->width = game.width;

    throw IllegalArgument();
}

Game& Game::operator=(const Game& game)
{
    this->height = game.height;
    this->width = game.width;

    throw IllegalArgument();

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
        return new Medic(type, health, ammo, range, power);
        break;
    
    default:
        // ToDo: what should be the default ?
        return new Character(type, health, ammo, range, power);
        break;
    }
}

void Game::addCharacter(const GridPoint& coordinates, Character *character)
{
    if ((coordinates.row < 0) || (coordinates.col < 0) || (coordinates.row >= this->height) || (coordinates.col >= this->width))
    {
        throw IllegalCell();
    }

    if (NULL != this->board[coordinates.col][coordinates.row])
    {
        throw CellOccupied();
    }

    this->board[coordinates.col][coordinates.row] = character;
}
