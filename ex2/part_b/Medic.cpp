#include "Medic.h"

using namespace mtm;

const units_t MOVE_RANGE = 5;
const units_t RELOAD_AMMO = 5;
const units_t AMMO_TO_ATTACK_RIVAL = 1;
const units_t AMMO_TO_ATTACK_ALLY = 0;

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

bool Medic::isAttackInRange(int range) const
{
    if (range > MOVE_RANGE)
    {
        return false;
    }

    return true;
}

bool Medic::isEnoughAmmo(Team defender_team) const
{
    units_t min_ammo_for_attack = AMMO_TO_ATTACK_ALLY;

    if (defender_team != this->team)
    {
        min_ammo_for_attack = AMMO_TO_ATTACK_RIVAL;
    }

    if (this->ammo < min_ammo_for_attack)
    {
        return false;
    }

    return true;
}

bool Medic::canAttack(bool is_destination_empty, bool is_destination_equals_source) const
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

char Medic::getSymbol() const
{
    // ToDo: Should it be implemented this way ?
    return this->team == POWERLIFTERS ? 'M' : 'm';
}
