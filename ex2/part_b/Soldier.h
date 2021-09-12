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

        ~Soldier() = default;
        Soldier(const Soldier& soldier) = default;
        Soldier& operator=(const Soldier& soldier) = default;

        bool canAttack(Character* character, const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const override;

        units_t getImpactRange() const override;

        units_t attack(Team defender_team, const GridPoint& coordinates_dst, const GridPoint& coordinates_attack) override;

        Character* clone() const override;
    };
}

#endif // _SOLDIER_H
