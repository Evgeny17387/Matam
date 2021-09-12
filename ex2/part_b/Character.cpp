#include "Character.h"

using namespace mtm;

Character::Character(Team team, units_t health, units_t ammo, units_t range, units_t power, units_t move_range, units_t reload_ammo):
    health(health), move_range(move_range), reload_ammo(reload_ammo), team(team), ammo(ammo), range(range), power(power)
{
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

units_t Character::getHealth() const
{
    return this->health;
}

void Character::updateHealth(units_t impact)
{
    this->health += impact;
}

int Character::divideToClosestUpperInt(int number, int divider)
{
    return number % divider == 0 ? number / divider : number / divider + 1;
}
