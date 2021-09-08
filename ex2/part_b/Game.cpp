#include "Game.h"

using namespace mtm;

#include "Medic.h"

Game::Game(int height, int widht)
{
    this->height = height;
    this->widht = widht;
}

Game::~Game()
{
}

Game::Game(const Game& game)
{
    this->height = game.height;
    this->widht = game.widht;
}

Game& Game::operator=(const Game& game)
{
    this->height = game.height;
    this->widht = game.widht;

    return *this;
}

Character Game::makeCharacter(CharacterType type, Team team, units_t health, units_t ammo, units_t range, units_t power)
{
    if ((health <= 0) || (ammo < 0) || (range < 0) || (power < 0))
    {
        throw IllegalArgument();
    }

    switch (type)
    {
    case CharacterType::MEDIC:
        return Medic(health, ammo, range, power);
        break;
    
    default:
        // ToDo: what should be the default ?
        return Character(health, ammo, range, power);
        break;
    }
}

void Game::addCharacter(const GridPoint& coordinates, Character character)
{

}
