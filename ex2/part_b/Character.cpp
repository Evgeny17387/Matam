#include "Character.h"

using namespace mtm;

Character::Character(Team team, units_t health, units_t ammo, units_t range, units_t power, units_t move_range, units_t reload_ammo)
{
    this->team = team;
    this->health = health;
    this->ammo = ammo;
    this->range = range;
    this->power = power;
    this->move_range = move_range;
    this->reload_ammo = reload_ammo;
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
    this->reload_ammo = character.reload_ammo;
}

Character& Character::operator=(const Character& character)
{
    this->team = character.team;
    this->health = character.health;
    this->ammo = character.ammo;
    this->range = character.range;
    this->power = character.power;
    this->move_range = character.move_range;
    this->reload_ammo = character.reload_ammo;

    return *this;
}

int Character::getMoveRange() const
{
    return this->move_range;
}

void Character::reload()
{
    this->ammo += this->reload_ammo;
}

Team Character::getTeam() const
{
    return this->team;
}

char Character::getSymbol() const
{
    return this->team == POWERLIFTERS ? 'C' : 'c';
}
