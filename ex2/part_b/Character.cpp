#include "Character.h"

using namespace mtm;

Character::Character(Team team, units_t health, units_t ammo, units_t range, units_t power, units_t move_range)
{
    this->team = team;
    this->health = health;
    this->ammo = ammo;
    this->range = range;
    this->power = power;
    this->move_range = move_range;
}

Character::~Character()
{
}

Character::Character(const Character& character)
{
    this->team = character.team;
    this->health = character.health;
    this->ammo = character.ammo;
    this->range = character.range;
    this->power = character.power;
    this->move_range = character.move_range;
}

Character& Character::operator=(const Character& character)
{
    this->team = character.team;
    this->health = character.health;
    this->ammo = character.ammo;
    this->range = character.range;
    this->power = character.power;
    this->move_range = character.move_range;

    return *this;
}

int Character::GetMoveRange() const
{
    return this->move_range;
}

char Character::GetSymbol() const
{
    return this->team == POWERLIFTERS ? 'C' : 'c';
}
