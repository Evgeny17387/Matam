#include "Soldier.h"

using namespace mtm;

const units_t MOVE_RANGE = 3;

const units_t RELOAD_AMMO = 3;

const units_t AMMO_TO_ATTACK = 1;

const units_t NO_DAMAGE = 0;

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

bool Soldier::isOnTheSameLine(const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const
{
    if ((coordinates_src.col != coordinates_dst.col) && (coordinates_src.row != coordinates_dst.row))
    {
        return false;
    }

    // ToDo: this can be both line and point, is that o.k. ?
    return true;
}

bool Soldier::isAttackInRange(const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const
{
    // ToDo: since all have it, maybe it can we common somhow ?
    int attack_range = GridPoint::distance(coordinates_src, coordinates_dst);

    if (attack_range > this->range)
    {
        return false;
    }

    if (!isOnTheSameLine(coordinates_src, coordinates_dst))
    {
        return false;
    }

    return true;
}

bool Soldier::isEnoughAmmo(Team defender_team) const
{
    units_t min_ammo_for_attack = AMMO_TO_ATTACK;

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

units_t Soldier::getImpactRange() const
{
    return this->range / 3 + 1;
}

units_t Soldier::attack(Team defender_team, const GridPoint& coordinates_dst, const GridPoint& coordinates_attack)
{
    this->ammo -= AMMO_TO_ATTACK;

    if (defender_team == this->team)
    {
        return NO_DAMAGE;
    }
    else
    {
        if (coordinates_dst == coordinates_attack)
        {
            return -this->power;
        }
        else
        {
            int attack_range = GridPoint::distance(coordinates_dst, coordinates_attack);

            // ToDo: duplicate logic with getImpactRange
            int max_range_affect = this->range / 3 + 1;

            units_t reduced_damage = this->power / 2 + 1;

            if (attack_range > max_range_affect)
            {
                return NO_DAMAGE;
            }
            else
            {
                return -reduced_damage;
            }
        }
    }
}
