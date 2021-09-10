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

        ~Sniper();

        Sniper(const Sniper& sniper);

        Sniper& operator=(const Sniper& sniper);

        char getSymbol() const override;

        bool isAttackInRange(const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const override;

        bool isEnoughAmmo(Team defender_team) const override;

        bool canAttack(bool is_destination_empty, bool is_destination_equals_source, Team defender_team) const override;

        units_t getImpactRange() const override;

        units_t attack(Team defender_team, const GridPoint& coordinates_dst, const GridPoint& coordinates_attack) override;

        Character* clone() const override;
    };
}

#endif // _SNIPER_H
