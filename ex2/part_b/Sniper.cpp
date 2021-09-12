#include "Sniper.h"

namespace mtm
{
    const units_t MOVE_RANGE = 4;

    const units_t RELOAD_AMMO = 2;

    const units_t AMMO_PER_ATTACK = 1;

    const units_t ATTACK_IMPACT_ONLY_SINGLE_CELL = 0;

    const int DOUBLE_IMPACT_SHOT_COUNT = 3;

    const int DOUBLE_IMPACT_SHOT = 2;

    const char SYMBOL = 'N';

    Sniper::Sniper(Team team, units_t health, units_t ammo, units_t range, units_t power):
        Character(team, health, ammo, range, power, MOVE_RANGE, RELOAD_AMMO, AMMO_PER_ATTACK, SYMBOL)
    {
        this->regular_shots_count = 0;
    }

    bool Sniper::isAttackInRange(const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const
    {
        int attack_range = GridPoint::distance(coordinates_src, coordinates_dst);

        if (attack_range > this->range)
        {
            return false;
        }

        int min_attach_range = divideToClosestUpperInt(this->range, 2);

        if (attack_range < min_attach_range)
        {
            return false;
        }

        return true;
    }

    bool Sniper::canAttack(Character* character, const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const
    {
        if (character == NULL)
        {
            return false;
        }

        if (character->getTeam() == this->getTeam())
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
        this->regular_shots_count++;

        if (this->regular_shots_count == DOUBLE_IMPACT_SHOT_COUNT)
        {
            this->regular_shots_count = 0;
            return -this->power * DOUBLE_IMPACT_SHOT;
        }
        else
        {
            return -this->power;
        }
    }

    Character* Sniper::clone() const
    {
        return new Sniper(*this);
    }
}
