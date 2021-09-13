#include "Medic.h"

using std::shared_ptr;

namespace mtm
{
    const units_t MOVE_RANGE = 5;

    const units_t RELOAD_AMMO = 5;

    const units_t AMMO_PER_ATTACK_RIVAL = 1;
    const units_t AMMO_PER_ATTACK_ALLY = 0;

    const char SYMBOL = 'M';

    Medic::Medic(Team team, units_t health, units_t ammo, units_t range,
        units_t power):
        Character(team, health, MOVE_RANGE, RELOAD_AMMO, AMMO_PER_ATTACK_RIVAL,
            SYMBOL, ammo, range, power)
    {
    }

    Character* Medic::clone() const
    {
        return new Medic(*this);
    }

    bool Medic::isEnoughAmmo(Character* character) const
    {
        units_t min_ammo_for_attack = AMMO_PER_ATTACK_ALLY;

        if (character == NULL)
        {
            min_ammo_for_attack = AMMO_PER_ATTACK_RIVAL;
        }
        else if (character->getTeam() != this->getTeam())
        {
            min_ammo_for_attack = AMMO_PER_ATTACK_RIVAL;
        }

        if (this->ammo < min_ammo_for_attack)
        {
            return false;
        }

        return true;
    }

    bool Medic::canAttack(Character* character,
        const GridPoint& coordinates_src,
        const GridPoint& coordinates_dst) const
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

    void Medic::attack(
        std::vector<std::vector<std::shared_ptr<Character>>>& board,
        const GridPoint& coordinates_dst)
    {
        shared_ptr<Character> defender =
            board[coordinates_dst.col][coordinates_dst.row];

        if (defender->getTeam() != this->getTeam())
        {
            this->ammo -= AMMO_PER_ATTACK_RIVAL;
        }

        units_t impact = 0;

        if (defender->getTeam() == this->getTeam())
        {
            impact = this->power;
        }
        else
        {
            impact = -this->power;
        }

        defender->updateHealth(impact);
    }
}
