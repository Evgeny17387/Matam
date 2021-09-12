#ifndef _SNIPER_H
#define _SNIPER_H

#include "Character.h"

namespace mtm
{
    class Sniper: public Character
    {
    private:

        int regular_shots_count;

    public:

        Sniper(Team team, units_t health, units_t ammo, units_t range, units_t power);

        ~Sniper() = default;
        Sniper(const Sniper& sniper) = default;
        Sniper& operator=(const Sniper& sniper) = default;

        bool isAttackInRange(const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const override;

        bool canAttack(Character* character, const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const override;

        units_t getImpactRange() const override;

        void chargeAttackAmmoCost(Character* defender) override;

        units_t attack(Team defender_team, const GridPoint& coordinates_dst, const GridPoint& coordinates_attack) override;

        Character* clone() const override;
    };
}

#endif // _SNIPER_H
