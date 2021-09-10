#ifndef _SOLDIER_H
#define _SOLDIER_H

#include "Character.h"

namespace mtm
{
    class Soldier: public Character
    {
    private:

        bool isOnTheSameLine(const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const;

    public:

        Soldier(Team team, units_t health, units_t ammo, units_t range, units_t power);

        ~Soldier();

        Soldier(const Soldier& soldier);

        Soldier& operator=(const Soldier& soldier);

        char getSymbol() const override;

        bool isAttackInRange(const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const override;

        bool isEnoughAmmo(Team defender_team) const override;

        bool canAttack(bool is_destination_empty, bool is_destination_equals_source) const override;

        units_t getImpactRange() const override;

        units_t attack(Team defender_team, const GridPoint& coordinates_dst, const GridPoint& coordinates_attack) override;
    };
}

#endif // _SOLDIER_H
