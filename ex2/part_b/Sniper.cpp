#include "Sniper.h"

using std::shared_ptr;

namespace mtm
{
    const units_t MOVE_RANGE = 4;

    const units_t RELOAD_AMMO = 2;

    const units_t AMMO_PER_ATTACK = 1;

    const int DOUBLE_IMPACT_SHOT_COUNT = 3;

    const int DOUBLE_IMPACT_SHOT = 2;

    const char SYMBOL = 'N';

    Sniper::Sniper(Team team, units_t health, units_t ammo, units_t range,
        units_t power): Character(team, health, MOVE_RANGE, RELOAD_AMMO,
        AMMO_PER_ATTACK, SYMBOL, ammo, range, power)
    {
        this->regular_shots_count = 0;
    }

    Character* Sniper::clone() const
    {
        return new Sniper(*this);
    }

    bool Sniper::isAttackInRange(const GridPoint& coordinates_src,
        const GridPoint& coordinates_dst) const
    {
        int attack_range = GridPoint::distance(coordinates_src,
            coordinates_dst);

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

    bool Sniper::canAttack(Character* character,
        const GridPoint& coordinates_src,
        const GridPoint& coordinates_dst) const
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

    void Sniper::attack(
        std::vector<std::vector<std::shared_ptr<Character>>>& board,
        const GridPoint& coordinates_dst)
    {
        shared_ptr<Character> defender =
            board[coordinates_dst.col][coordinates_dst.row];

        this->ammo -= AMMO_PER_ATTACK;

        this->regular_shots_count++;

        units_t impact = 0;

        if (this->regular_shots_count == DOUBLE_IMPACT_SHOT_COUNT)
        {
            this->regular_shots_count = 0;
            impact = -this->power * DOUBLE_IMPACT_SHOT;
        }
        else
        {
            impact = -this->power;
        }

        defender->updateHealth(impact);
    }
}
