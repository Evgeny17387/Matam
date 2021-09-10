#include "Sniper.h"

using namespace mtm;

const units_t MOVE_RANGE = 4;

const units_t RELOAD_AMMO = 2;

const units_t AMMO_PER_ATTACK = 1;

const units_t ATTACK_IMPACT_ONLY_SINGLE_CELL = 0;

Sniper::Sniper(Team team, units_t health, units_t ammo, units_t range, units_t power): Character(team, health, ammo, range, power, MOVE_RANGE, RELOAD_AMMO)
{
}

Sniper::~Sniper()
{
    // ToDo: how to implement ?
}

Sniper::Sniper(const Sniper& sniper): Character(sniper)
{
}

Sniper& Sniper::operator=(const Sniper& sniper)
{
    // ToDo: how to implement ?
    throw NotImplementedYet();
    return *this;
}

char Sniper::getSymbol() const
{
    // ToDo: Should it be implemented this way ?
    return this->team == POWERLIFTERS ? 'N' : 'n';
}

bool Sniper::isAttackInRange(const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const
{
    int attack_range = GridPoint::distance(coordinates_src, coordinates_dst);

    if (attack_range > this->range)
    {
        return false;
    }

    int min_attach_range = this->range / 2 + 1;

    if (attack_range < min_attach_range)
    {
        return false;
    }

    return true;
}

bool Sniper::isEnoughAmmo(Team defender_team) const
{
    if (this->ammo < AMMO_PER_ATTACK)
    {
        return false;
    }

    return true;
}

bool Sniper::canAttack(bool is_destination_empty, bool is_destination_equals_source, Team defender_team) const
{
    if (is_destination_empty)
    {
        return false;
    }

    if (defender_team == this->team)
    {
        return false;
    }

    return true;
}

units_t Sniper::getImpactRange() const
{
    return ATTACK_IMPACT_ONLY_SINGLE_CELL;
}

units_t Sniper::attack(Team defender_team, const GridPoint& coordinates_dst, const GridPoint& coordinates_attack)
{
    this->ammo -= AMMO_PER_ATTACK;

    if (defender_team == this->team)
    {
        return this->power;
    }
    else
    {
        return -this->power;
    }
}
