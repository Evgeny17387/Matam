#include "Soldier.h"

namespace mtm
{
    const units_t MOVE_RANGE = 3;

    const units_t RELOAD_AMMO = 3;

    const units_t AMMO_PER_ATTACK = 1;

    const units_t NO_DAMAGE = 0;

    const char SYMBOL = 'S';

    Soldier::Soldier(Team team, units_t health, units_t ammo, units_t range, units_t power):
        Character(team, health, ammo, range, power, MOVE_RANGE, RELOAD_AMMO, AMMO_PER_ATTACK, SYMBOL)
    {
    }

    bool Soldier::isOnTheSameLine(const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const
    {
        if ((coordinates_src.col != coordinates_dst.col) && (coordinates_src.row != coordinates_dst.row))
        {
            return false;
        }

        return true;
    }

    bool Soldier::canAttack(Character* character, const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const
    {
        if (!isOnTheSameLine(coordinates_src, coordinates_dst))
        {
            return false;
        }

        return true;
    }

    units_t Soldier::getImpactRange() const
    {
        return divideToClosestUpperInt(this->range, 3);
    }

    void Soldier::chargeAttackAmmoCost(Character* defender)
    {
        this->ammo -= AMMO_PER_ATTACK;
    }

    units_t Soldier::attack(Team defender_team, const GridPoint& coordinates_dst, const GridPoint& coordinates_attack)
    {
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
                int max_range_affect = divideToClosestUpperInt(this->range, 3);

                units_t reduced_damage = divideToClosestUpperInt(this->power, 2);

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

    Character* Soldier::clone() const
    {
        return new Soldier(*this);
    }
}
