#ifndef _MEDIC_H
#define _MEDIC_H

#include "Character.h"

namespace mtm
{
    class Medic: public Character
    {
    private:

    public:

        Medic(Team team, units_t health, units_t ammo, units_t range, units_t power);

        ~Medic();

        Medic(const Medic& medic);

        Medic& operator=(const Medic& medic);

        char getSymbol() const override;

        bool isAttackInRange(const GridPoint& coordinates_src, const GridPoint& coordinates_dst) const override;

        bool isEnoughAmmo(Team defender_team) const override;

        bool canAttack(bool is_destination_empty, bool is_destination_equals_source, Team defender_team) const override;

        units_t getImpactRange() const override;

        units_t attack(Team defender_team, const GridPoint& coordinates_dst, const GridPoint& coordinates_attack) override;

        Character* clone() const override;
    };
}

#endif // _MEDIC_H
