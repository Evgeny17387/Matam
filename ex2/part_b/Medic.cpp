#include "Medic.h"

namespace mtm
{
    const units_t MOVE_RANGE = 5;

    const units_t RELOAD_AMMO = 5;

    const units_t AMMO_PER_ATTACK_RIVAL = 1;
    const units_t AMMO_PER_ATTACK_ALLY = 0;

    const units_t ATTACK_IMPACT_ONLY_SINGLE_CELL = 0;

    const char SYMBOL = 'M';

    Medic::Medic(Team team, units_t health, units_t ammo, units_t range, units_t power):
        Character(team, health, ammo, range, power, MOVE_RANGE, RELOAD_AMMO, AMMO_PER_ATTACK_RIVAL, SYMBOL)
    {
    }

    bool Medic::isEnoughAmmo(Character* character) const
    {
        units_t min_ammo_for_attack = AMMO_PER_ATTACK_ALLY;

        if (character == NULL)
        {
            min_ammo_for_attack = AMMO_PER_ATTACK_RIVAL;
        }
        else if (character->getTeam() != this->team)
        {
            min_ammo_for_attack = AMMO_PER_ATTACK_RIVAL;
        }

        if (this->ammo < min_ammo_for_attack)
        {
            return false;
        }

        return true;
    }

    bool Medic::canAttack(Character* character, const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const
    {
        if (character == NULL)
        {
            return false;
        }

        if (coordinates_src == coordinates_dst)
        {
            return false;
        }

        return true;
    }

    units_t Medic::getImpactRange() const
    {
        return ATTACK_IMPACT_ONLY_SINGLE_CELL;
    }

    void Medic::chargeAttackAmmoCost(Character* defender)
    {
        if (defender->getTeam() != this->team)
        {
            this->ammo -= AMMO_PER_ATTACK_RIVAL;
        }
    }

    units_t Medic::attack(Team defender_team, const GridPoint& coordinates_dst, const GridPoint& coordinates_attack)
    {
        if (defender_team == this->team)
        {
            return this->power;
        }
        else
        {
            return -this->power;
        }
    }

    Character* Medic::clone() const
    {
        return new Medic(*this);
    }
}
