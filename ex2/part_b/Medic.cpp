#include "Medic.h"

using namespace mtm;

const units_t MOVE_RANGE = 5;

const units_t RELOAD_AMMO = 5;

const units_t AMMO_PER_ATTACK_RIVAL = 1;
const units_t AMMO_PER_ATTACK_ALLY = 0;

const units_t ATTACK_IMPACT_ONLY_SINGLE_CELL = 0;

Medic::Medic(Team team, units_t health, units_t ammo, units_t range, units_t power): Character(team, health, ammo, range, power, MOVE_RANGE, RELOAD_AMMO)
{
}

Medic::~Medic()
{
    // ToDo: how to implement ?
}

Medic::Medic(const Medic& medic): Character(medic)
{
}

Medic& Medic::operator=(const Medic& medic)
{
    // ToDo: how to implement ?
    throw NotImplementedYet();
    return *this;
}

char Medic::getSymbol() const
{
    // ToDo: Should it be implemented this way ?
    return this->team == POWERLIFTERS ? 'M' : 'm';
}

bool Medic::isAttackInRange(const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const
{
    int attack_range = GridPoint::distance(coordinates_src, coordinates_dst);

    if (attack_range > this->range)
    {
        return false;
    }

    return true;
}

bool Medic::isEnoughAmmo(Team defender_team) const
{
    units_t min_ammo_for_attack = AMMO_PER_ATTACK_ALLY;

    if (defender_team != this->team)
    {
        min_ammo_for_attack = AMMO_PER_ATTACK_RIVAL;
    }

    if (this->ammo < min_ammo_for_attack)
    {
        return false;
    }

    return true;
}

bool Medic::canAttack(bool is_destination_empty, bool is_destination_equals_source, Team defender_team) const
{
    if (is_destination_empty)
    {
        return false;
    }

    if (is_destination_equals_source)
    {
        return false;
    }

    return true;
}

units_t Medic::getImpactRange() const
{
    return ATTACK_IMPACT_ONLY_SINGLE_CELL;
}

units_t Medic::attack(Team defender_team, const GridPoint& coordinates_dst, const GridPoint& coordinates_attack)
{
    if (defender_team != this->team)
    {
        this->ammo -= AMMO_PER_ATTACK_RIVAL;
    }

    if (defender_team == this->team)
    {
        return this->power;
    }
    else
    {
        return -this->power;
    }
}
