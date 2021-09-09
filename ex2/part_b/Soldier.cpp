#include "Soldier.h"

using namespace mtm;

const units_t MOVE_RANGE = 3;

const units_t RELOAD_AMMO = 3;

const units_t AMMO_TO_ATTACK_RIVAL = 1;
const units_t AMMO_TO_ATTACK_ALLY = 1;

Soldier::Soldier(Team team, units_t health, units_t ammo, units_t range, units_t power): Character(team, health, ammo, range, power, MOVE_RANGE, RELOAD_AMMO)
{
}

Soldier::~Soldier()
{
    // ToDo: how to implement ?
}

Soldier::Soldier(const Soldier& soldier): Character(soldier)
{
}

Soldier& Soldier::operator=(const Soldier& soldier)
{
    // ToDo: how to implement ?
    throw NotImplementedYet();
    return *this;
}

char Soldier::getSymbol() const
{
    // ToDo: Should it be implemented this way ?
    return this->team == POWERLIFTERS ? 'S' : 's';
}

bool Soldier::isEnoughAmmo(Team defender_team) const
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

bool Soldier::canAttack(bool is_destination_empty, bool is_destination_equals_source) const
{
    return true;
}

units_t Soldier::attack(Team defender_team)
{
    if (defender_team == this->team)
    {
        return this->power;
    }
    else
    {
        this->ammo -= AMMO_TO_ATTACK_RIVAL;
        return -this->power;
    }
}
